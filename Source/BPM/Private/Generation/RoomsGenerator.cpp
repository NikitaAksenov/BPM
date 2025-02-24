// Fill out your copyright notice in the Description page of Project Settings.


#include "Generation/RoomsGenerator.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Generation/RoomEntrance.h"
#include "Generation/RoomGenerationDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


DEFINE_LOG_CATEGORY(LogRoomsGenerator);

UE_DISABLE_OPTIMIZATION
ARoomsGenerator::ARoomsGenerator()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ARoomsGenerator::BeginPlay()
{
	Super::BeginPlay();

	StartGeneration(LevelType, RoomGenerationDataAssetClass);
	
}

void ARoomsGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoomsGenerator::StartGeneration(ELevelType InLevelType, TSubclassOf<URoomGenerationDataAsset> InRoomGenerationDataAssetClass)
{
	UE_LOG(LogRoomsGenerator, Display, TEXT("Generation started, level type %s"), *UEnum::GetValueAsString(InLevelType));

	RoomGenerationDataAssets.Empty();
	SpawnedRoomsCount = 0;

	RoomGenerationDataAssets = GetRoomGenerationDataAssets(InLevelType, InRoomGenerationDataAssetClass);
	checkf(!RoomGenerationDataAssets.IsEmpty(), TEXT("Failed to get room generation data assets. LevelType: %s"),
		*UEnum::GetValueAsString(InLevelType));

	GenerateNextRoom();
}

void ARoomsGenerator::GenerateNextRoom()
{
	if (!RoomGenerationDataAssets.IsValidIndex(SpawnedRoomsCount))
	{
		UE_LOG(LogRoomsGenerator, Log, TEXT("No room generation data assets left"));
		return;
	}
	
	FRoomGenerationData RoomGenerationData = RoomGenerationDataAssets[SpawnedRoomsCount]->GetRoomGenerationData();

	TArray<ERoomEntranceSide> RoomEntranceSides;
	RoomGenerationData.RoomEntrancesTransforms.GetKeys(RoomEntranceSides);

	const ERoomEntranceSide RandomSide = RoomEntranceSides[FMath::RandRange(0, RoomEntranceSides.Num() - 1)];
	const FTransform RandomSideTransform = RoomGenerationData.RoomEntrancesTransforms[RandomSide];

	CurrentRoomEntranceInfo = FRoomEntranceInfo(RandomSide, RandomSideTransform);

	FreeEntrances.Empty();
	TArray<AActor*> RoomEntranceActors;
	UGameplayStatics::GetAllActorsOfClass(this, ARoomEntrance::StaticClass(), RoomEntranceActors);
	for (AActor* RoomEntranceActor : RoomEntranceActors)
	{
		if (ARoomEntrance* RoomEntrance = Cast<ARoomEntrance>(RoomEntranceActor))
		{
			if (!RoomEntrance->IsConnected())
			{
				FreeEntrances.Add(RoomEntrance);
			}
		}
	}

	if (FreeEntrances.IsEmpty() &&
		SpawnedRoomsCount != 0)
	{
		UE_LOG(LogRoomsGenerator, Error, TEXT("No free entrances left"));
		return;
	}

	FVector EntranceLocation = FVector::ZeroVector;
	FRotator EntranceRotation = FRotator::ZeroRotator;

	FVector SpawnLocation = FVector::ZeroVector;
	FRotator SpawnRotation = FRotator::ZeroRotator;

	bool bCollisionCheck = false;
	for (ARoomEntrance* FreeEntrance : FreeEntrances)
	{
		CurrentRoomEntrance = FreeEntrance;
		
		EntranceLocation = FreeEntrance->GetActorLocation() + FreeEntrance->GetActorForwardVector() * RoomOffset;
		EntranceRotation = FreeEntrance->GetActorRotation();

		SpawnRotation.Pitch = 0.f;
		SpawnRotation.Roll = 0.f;
		SpawnRotation.Yaw = EntranceRotation.Yaw - RandomSideTransform.Rotator().Yaw - 180.f;
		
		SpawnLocation = EntranceLocation - SpawnRotation.RotateVector(RandomSideTransform.GetLocation());

		const FVector TraceStart = SpawnLocation + SpawnRotation.RotateVector(RoomGenerationData.RoomBoundsOrigin);
		const FVector TraceEnd = TraceStart;
		const FVector TraceBoxHalfSize = RoomGenerationData.RoomBoundsExtent;
		TArray<AActor*> ActorsToIgnore;
		FHitResult HitResult;
		bCollisionCheck = UKismetSystemLibrary::BoxTraceSingle(this, TraceStart, TraceEnd, TraceBoxHalfSize, SpawnRotation, TraceTypeQuery, false, ActorsToIgnore, DrawDebugType, HitResult, true);

		DrawDebugBox(GetWorld(), TraceStart, TraceBoxHalfSize, FColor::Yellow, false, 5.f);

		if (bCollisionCheck)
		{
			break;
		}
	}

	//checkf(!bCollisionCheck, TEXT("Failed to spawn room without colliding"));

	LoadRoom(RoomGenerationData.RoomData.World, SpawnLocation, SpawnRotation);
}

void ARoomsGenerator::LoadRoom(TSoftObjectPtr<UWorld> InWorld, const FVector& InLocation, const FRotator& InRotation)
{
	bool bSuccess = false;
	ULevelStreamingDynamic* LevelStreamingDynamic = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(this, InWorld, InLocation, InRotation, bSuccess);

	if (bSuccess)
	{
		LevelStreamingDynamic->OnLevelShown.AddDynamic(this, &ARoomsGenerator::OnRoomShown);
	}
	else
	{
		UE_LOG(LogRoomsGenerator, Error, TEXT("Failed to load room instance for %s"), *InWorld->GetName());
	}
}

void ARoomsGenerator::OnRoomShown()
{
	SpawnedRoomsCount++;

	TArray<AActor*> RoomEntranceActors;
	UGameplayStatics::GetAllActorsOfClass(this, ARoomEntrance::StaticClass(), RoomEntranceActors);

	for (AActor* RoomEntranceActor : RoomEntranceActors)
	{
		if (ARoomEntrance* FreeRoomEntrance = Cast<ARoomEntrance>(RoomEntranceActor))
		{
			if (!OldRoomEntrances.Contains(FreeRoomEntrance))
			{
				if (CurrentRoomEntrance &&
					CurrentRoomEntranceInfo.RoomEntranceSide == FreeRoomEntrance->GetSide())
				{
					CurrentRoomEntrance->SetConnectedEntrance(FreeRoomEntrance);
					FreeRoomEntrance->SetConnectedEntrance(CurrentRoomEntrance);
				}
				else
				{
					OldRoomEntrances.Add(FreeRoomEntrance);
				}
			}
		}
	}

	GenerateNextRoom();
}

TArray<URoomGenerationDataAsset*> ARoomsGenerator::GetRoomGenerationDataAssets(
	ELevelType InLevelType,
	TSubclassOf<URoomGenerationDataAsset> InRoomGenerationDataAssetClass) const
{
	TArray<URoomGenerationDataAsset*> OutRoomGenerationDataAssets;

	if (!InRoomGenerationDataAssetClass.Get())
	{
		return OutRoomGenerationDataAssets;
	}

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
	
	const FTopLevelAssetPath AssetPath = UKismetSystemLibrary::GetClassTopLevelAssetPath(InRoomGenerationDataAssetClass.Get());
	TArray<FAssetData> AssetDatas;
	if (!AssetRegistry.GetAssetsByClass(AssetPath, AssetDatas))
	{
		return OutRoomGenerationDataAssets;
	}
	
	for (const FAssetData& AssetData : AssetDatas)
	{
		if (URoomGenerationDataAsset* RoomGenerationDataAsset = Cast<URoomGenerationDataAsset>(AssetData.GetAsset()))
		{
			if (RoomGenerationDataAsset->IsEnabled() &&
				RoomGenerationDataAsset->GetRoomGenerationData().RoomData.Level == InLevelType)
			{
				OutRoomGenerationDataAssets.Add(RoomGenerationDataAsset);
			}
		}
	}

	return OutRoomGenerationDataAssets;
}
UE_ENABLE_OPTIMIZATION
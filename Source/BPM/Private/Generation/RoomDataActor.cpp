// Fill out your copyright notice in the Description page of Project Settings.


#include "Generation/RoomDataActor.h"

#include "Engine/LevelBounds.h"
#include "Generation/RoomEntrance.h"
#include "Generation/RoomGenerationDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


ARoomDataActor::ARoomDataActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ARoomDataActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARoomDataActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

#if WITH_EDITOR
void ARoomDataActor::UpdateRoomData()
{
	UWorld* World = GetWorld();
	if (World &&
		(RoomData.World.IsNull() || RoomData.World != World))
	{
		RoomData.World = World;
		MarkPackageDirty();
	}

	if (RoomGenerationDataAsset)
	{
		// Get room entrances transforms
		TMap<ERoomEntranceSide, FTransform> RoomEntrancesTransforms;
		TArray<AActor*> RoomEntranceActors;
		UGameplayStatics::GetAllActorsOfClass(this, ARoomEntrance::StaticClass(), RoomEntranceActors);
		for (AActor* RoomEntranceActor : RoomEntranceActors)
		{
			if (const ARoomEntrance* RoomEntrance = Cast<ARoomEntrance>(RoomEntranceActor))
			{
				RoomEntrancesTransforms.Add(RoomEntrance->GetSide(), RoomEntrance->GetActorTransform());
			}
		}

		// Get room bounds
		FVector RoomBoundsOrigin, RoomBoundsExtent;
		TArray<AActor*> LevelBoundsActors;
		UGameplayStatics::GetAllActorsOfClass(this, ALevelBounds::StaticClass(), LevelBoundsActors);
		if (!LevelBoundsActors.IsEmpty())
		{
			if (ALevelBounds* LevelBounds = Cast<ALevelBounds>(LevelBoundsActors[0]))
			{
				LevelBounds->GetActorBounds(false, RoomBoundsOrigin, RoomBoundsExtent);
			}
		}
		
		FRoomGenerationData RoomGenerationData;
		RoomGenerationData.RoomData = RoomData;
		RoomGenerationData.RoomEntrancesTransforms = RoomEntrancesTransforms;
		RoomGenerationData.RoomBoundsOrigin = RoomBoundsOrigin;
		RoomGenerationData.RoomBoundsExtent = RoomBoundsExtent;

		if (RoomGenerationData.IsValid())
		{
			UKismetSystemLibrary::BeginTransaction("RoomDataActor", FText::FromString("Update room generation data asset"), nullptr);

			RoomGenerationDataAsset->SetRoomGenerationData(RoomGenerationData);

			UKismetSystemLibrary::TransactObject(RoomGenerationDataAsset);
			UKismetSystemLibrary::EndTransaction();
		}
	}
}
#endif

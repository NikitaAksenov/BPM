// Fill out your copyright notice in the Description page of Project Settings.


#include "Generation/RoomDataActor.h"

#include "Generation/RoomGenerationDataAsset.h"
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
		UKismetSystemLibrary::BeginTransaction("RoomDataActor", FText::FromString("Update room generation data asset"), nullptr);
		FRoomGenerationData RoomGenerationData;
		RoomGenerationData.RoomData = RoomData;
		
		RoomGenerationDataAsset->SetRoomGenerationData(RoomGenerationData);

		UKismetSystemLibrary::TransactObject(RoomGenerationDataAsset);
		UKismetSystemLibrary::EndTransaction();
	}
}
#endif

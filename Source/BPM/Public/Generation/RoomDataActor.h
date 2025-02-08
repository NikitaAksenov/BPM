// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GenerationTypes.h"

#include "RoomDataActor.generated.h"


class URoomGenerationDataAsset;


UCLASS()
class BPM_API ARoomDataActor : public AActor
{
	GENERATED_BODY()
	
public:
	ARoomDataActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	
protected:
	UPROPERTY(EditInstanceOnly, Category = "RoomData")
	FRoomData RoomData;

	UPROPERTY(EditInstanceOnly, Category = "RoomData")
	TObjectPtr<URoomGenerationDataAsset> RoomGenerationDataAsset;

#if WITH_EDITOR
public:
	UFUNCTION(CallInEditor, Category = "RoomData")
	void UpdateRoomData();
#endif
};

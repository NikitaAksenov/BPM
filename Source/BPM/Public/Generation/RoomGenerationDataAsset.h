// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "GenerationTypes.h"

#include "RoomGenerationDataAsset.generated.h"


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class BPM_API URoomGenerationDataAsset : public UDataAsset
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	bool bEnabled;

	UPROPERTY(VisibleAnywhere)
	FRoomGenerationData RoomGenerationData;

public:
	UFUNCTION(BlueprintCallable, Category = "RoomGeneration")
	FORCEINLINE bool IsEnabled() const { return bEnabled; }
	
	UFUNCTION(BlueprintCallable, Category = "RoomGeneration")
	FORCEINLINE FRoomGenerationData GetRoomGenerationData() const { return RoomGenerationData; }

	UFUNCTION(BlueprintCallable, Category = "RoomGeneration")
	FORCEINLINE void SetRoomGenerationData(const FRoomGenerationData& InRoomGenerationData)
	{
		RoomGenerationData = InRoomGenerationData;
	}
};

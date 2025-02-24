// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GenerationTypes.h"
#include "Kismet/KismetSystemLibrary.h"

#include "RoomsGenerator.generated.h"


class ARoomEntrance;
class URoomGenerationDataAsset;


DECLARE_LOG_CATEGORY_EXTERN(LogRoomsGenerator, Log, All);


UCLASS()
class BPM_API ARoomsGenerator : public AActor
{
	GENERATED_BODY()
	
public:
	ARoomsGenerator();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "RoomsGenerator")
	ELevelType LevelType;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "RoomsGenerator")
	TSubclassOf<URoomGenerationDataAsset> RoomGenerationDataAssetClass;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "RoomsGenerator")
	float RoomOffset = 500.f;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "RoomsGenerator")
	TEnumAsByte<ETraceTypeQuery> TraceTypeQuery;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "RoomsGenerator")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType;

protected:
	UFUNCTION(BlueprintCallable, Category = "RoomsGenerator")
	void StartGeneration(ELevelType InLevelType, TSubclassOf<URoomGenerationDataAsset> InRoomGenerationDataAssetClass);

	void GenerateNextRoom();

	void LoadRoom(TSoftObjectPtr<UWorld> InWorld, const FVector& InLocation, const FRotator& InRotation);

	UFUNCTION()
	void OnRoomShown();
	
	UFUNCTION(BlueprintCallable, Category = "RoomGenerator")
	TArray<URoomGenerationDataAsset*> GetRoomGenerationDataAssets(
		ELevelType InLevelType,
		TSubclassOf<URoomGenerationDataAsset> InRoomGenerationDataAssetClass) const;

protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "RoomGenerator|Runtime")
	TArray<URoomGenerationDataAsset*> RoomGenerationDataAssets;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "RoomGenerator|Runtime")
	int SpawnedRoomsCount = 0;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "RoomGenerator|Runtime")
	TArray<ARoomEntrance*> FreeEntrances;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "RoomGenerator|Runtime")
	TSet<ARoomEntrance*> OldRoomEntrances;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "RoomGenerator|Runtime")
	FRoomEntranceInfo CurrentRoomEntranceInfo;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "RoomGenerator|Runtime")
	ARoomEntrance* CurrentRoomEntrance;

};

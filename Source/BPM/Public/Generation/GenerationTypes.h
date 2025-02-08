// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GenerationTypes.generated.h"


UENUM(BlueprintType)
enum class ERoomEntranceSide : uint8
{
	North,
	East,
	South,
	West
};

UENUM(BlueprintType)
enum class ELevelType : uint8
{
	Asgard_1,
	Asgard_2,
	AsgardCrypts,
	Vanaheim_1,
	Vanaheim_2,
	Svartalfheim_1,
	Svartalfheim_2,
	Helheim_1,
	Helheim_2
};

UENUM(BlueprintType)
enum class ERoomType : uint8
{
	Entrance,
	Normal,
	Bank,
	Boss,
	Challenge,
	BossChallenge,
	Well,
	Prestige,
	Treasure,
	Warp,
	Shop,
	Library,
	Armory,
	Tribute,
	Gambling,
	BlackMarket,
	Reroll,
	Miniboss,
	Dish,
	Pit,
	Tower
};


USTRUCT(BlueprintType)
struct FRoomData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> World = nullptr;

	UPROPERTY(EditAnywhere)
	ELevelType Level = ELevelType::Asgard_1;

	UPROPERTY(EditAnywhere)
	ERoomType RoomType = ERoomType::Normal;
};

USTRUCT(BlueprintType)
struct FRoomGenerationData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FRoomData RoomData;

	UPROPERTY(EditAnywhere)
	TMap<ERoomEntranceSide, FTransform> RoomEntrancesTransforms;

	UPROPERTY(EditAnywhere)
	FVector RoomBoundsOrigin;

	UPROPERTY(EditAnywhere)
	FVector RoomBoundsExtent;
};

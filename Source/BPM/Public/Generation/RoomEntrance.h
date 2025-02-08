// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GenerationTypes.h"

#include "RoomEntrance.generated.h"


class UArrowComponent;


UCLASS()
class BPM_API ARoomEntrance : public AActor
{
	GENERATED_BODY()
	
public:
	ARoomEntrance();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBillboardComponent> Billboard;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UArrowComponent> Arrow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "RoomEntrance")
	ERoomEntranceSide Side;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "RoomEntrance")
	ARoomEntrance* ConnectedEntrance;

public:
	UFUNCTION(BlueprintCallable, Category = "RoomEntrance")
	FORCEINLINE ERoomEntranceSide GetSide() const { return Side; }

	UFUNCTION(BlueprintCallable, Category = "RoomEntrance")
	FORCEINLINE bool IsConnected() const { return ConnectedEntrance != nullptr; }

	UFUNCTION(BlueprintCallable, Category = "RoomEntrance")
	FORCEINLINE ARoomEntrance* GetConnectedEntrance() const { return ConnectedEntrance; }

	UFUNCTION(BlueprintCallable, Category = "RoomEntrance")
	void SetConnectedEntrance(ARoomEntrance* InEntrance);
};

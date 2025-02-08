// Fill out your copyright notice in the Description page of Project Settings.


#include "Generation/RoomEntrance.h"

#include "Components/ArrowComponent.h"
#include "Components/BillboardComponent.h"


ARoomEntrance::ARoomEntrance()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(Root);
	
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Root);

	bRelevantForLevelBounds = false;
}

void ARoomEntrance::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARoomEntrance::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoomEntrance::SetConnectedEntrance(ARoomEntrance* InEntrance)
{
	ConnectedEntrance = InEntrance;

	if (ConnectedEntrance &&
		!ConnectedEntrance->IsConnected())
	{
		ConnectedEntrance->SetConnectedEntrance(this);
	}
}


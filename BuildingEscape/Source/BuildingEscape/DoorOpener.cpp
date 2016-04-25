// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "DoorOpener.h"


// Sets default values for this component's properties
UDoorOpener::UDoorOpener()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDoorOpener::BeginPlay()
{
	Super::BeginPlay();

	FString Name = GetOwner()->GetName();
	FString Rotation = GetOwner()->GetTransform().GetRotation().ToString();
	
	UE_LOG(LogTemp, Warning, TEXT("%s has rotation %s"), *Name, *Rotation);
}


// Called every frame
void UDoorOpener::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	float MaxZRotation = 90;
	float CurrentRotation = World->GetTimeSeconds()*40;

	if(CurrentRotation > MaxZRotation)
		return;

	FRotator Rotater = FRotator(0.0f, CurrentRotation, 0.0f);
	
	GetOwner()->SetActorRotation(Rotater);
}


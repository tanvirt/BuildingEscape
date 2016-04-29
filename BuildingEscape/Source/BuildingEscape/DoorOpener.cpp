// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "DoorOpener.h"

// Sets default values for this component's properties
UDoorOpener::UDoorOpener() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UDoorOpener::BeginPlay() {
	Super::BeginPlay();

	if (!PressurePlate)
		UE_LOG(LogTemp, Error, TEXT("Door Opener for %s is missing a Pressure Plate (Trigger Volume)"), *GetOwner()->GetName());

	OriginalYawAngle = GetOwner()->GetActorRotation().Yaw;
	CurrentYawAngle = OriginalYawAngle;
}

// Called every frame
void UDoorOpener::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOnPressurePlate() >= TriggerMass)
		OpenDoor();
	else
		CloseDoor();
}

void UDoorOpener::OpenDoor()  {
	if(CurrentYawAngle <= OriginalYawAngle + 90)
		CurrentYawAngle += 2;
	FRotator Rotater = FRotator(0.0f, CurrentYawAngle, 0.0f);
	GetOwner()->SetActorRotation(Rotater);
}

void UDoorOpener::CloseDoor() {
	if (CurrentYawAngle > OriginalYawAngle)
		CurrentYawAngle -= 3;
	FRotator Rotater = FRotator(0.0f, CurrentYawAngle, 0.0f);
	GetOwner()->SetActorRotation(Rotater);
}

float UDoorOpener::GetTotalMassOnPressurePlate() {
	TArray<AActor*> OverlappingActors;

	if (!PressurePlate) return -1.0f;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	float TotalMass = 0.0f;
	for (AActor* OverlappingActor : OverlappingActors)
		TotalMass += OverlappingActor->GetRootPrimitiveComponent()->GetMass();

	return TotalMass;
}

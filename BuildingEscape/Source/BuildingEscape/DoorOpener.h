// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "DoorOpener.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UDoorOpener : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDoorOpener();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	void OpenDoor();
	void CloseDoor();

private:
	float OriginalYawAngle = 0.0f;

	UPROPERTY(VisibleAnywhere)
	float CurrentYawAngle = 0.0f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;
	
	AActor* DoorOpeningActor = nullptr;
};

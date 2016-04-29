// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay() {
	Super::BeginPlay();

	SetUpPhysicsHandle();
	SetUpInputComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FVector LineTraceEnd = GetReachEnd();

	if (!PhysicsHandle) return;
	if (PhysicsHandle->GrabbedComponent)
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
}

void UGrabber::SetUpPhysicsHandle() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
		UE_LOG(LogTemp, Error, TEXT("Grabber for %s is missing a Physics Handle Component"), *GetOwner()->GetName());
}

void UGrabber::SetUpInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!InputComponent) {
		UE_LOG(LogTemp, Error, TEXT("Grabber for %s is missing an Input Component"), *GetOwner()->GetName());
		return;
	}

	InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
}

void UGrabber::Grab() {
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* Actor = HitResult.GetActor();

	if (Actor) {
		if (!PhysicsHandle) return;
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true
		);
	}
}

void UGrabber::Release() {
	if (!PhysicsHandle) return;
	if (PhysicsHandle->GrabbedComponent)
		PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() {
	FVector LineTraceStart = GetReachStart();
	FVector LineTraceEnd = GetReachEnd();

	return GetLineTraceHitResult(LineTraceStart, LineTraceEnd, ECollisionChannel::ECC_PhysicsBody);
}

FHitResult UGrabber::GetLineTraceHitResult(FVector LineTraceStart, FVector LineTraceEnd, ECollisionChannel CollisionChannel) {
	FCollisionQueryParams TraceParameters = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		LineTraceStart,
		LineTraceEnd,
		FCollisionObjectQueryParams(CollisionChannel),
		TraceParameters
	);

	return HitResult;
}

FVector UGrabber::GetReachStart() {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachEnd() {
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	return PlayerViewPointLocation + PlayerViewPointRotation.Vector()*Reach;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoorWithTrigger.h"

// Sets default values for this component's properties
UOpenDoorWithTrigger::UOpenDoorWithTrigger()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoorWithTrigger::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	UE_LOG(LogTemp, Warning, TEXT("Pawn is: %s"), *ActorThatOpens->GetName())
	
}


// Called every frame
void UOpenDoorWithTrigger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(TriggerPlate && TriggerPlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	if((GetWorld()->GetTimeSeconds() - DoorCloseDelay) > DoorLastOpened)
	{
		CloseDoor(DeltaTime);
	}
}
void UOpenDoorWithTrigger::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, 2);
	FRotator OpenDoor = GetOwner()->GetActorRotation();
	OpenDoor.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(OpenDoor);
}
void UOpenDoorWithTrigger::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::FInterpConstantTo(CurrentYaw, InitialYaw, DeltaTime, DoorCloseSpeed);
	FRotator OpenDoor = GetOwner()->GetActorRotation();
	OpenDoor.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(OpenDoor);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoorTest.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UOpenDoorTest::UOpenDoorTest()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UOpenDoorTest::BeginPlay()
{
	Super::BeginPlay();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;

	FindPressurePlate();
	FindAudioComponent();	
}

void UOpenDoorTest::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if(!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no audio component"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoorTest::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(TotalMassOfActors() > DoorOpenMass)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if((GetWorld()->GetTimeSeconds() - DoorLastOpened) > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoorTest::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime* DoorOpenSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	if(!AudioComponent) {return;}
	if(!DoorOpened)
	{
		AudioComponent->Play();
		DoorClosed = false;
		DoorOpened = true;
	}
}

void UOpenDoorTest::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime* DoorOpenSpeed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);

	if(!AudioComponent) {return;}
	if(!DoorClosed)
	{
		AudioComponent->Play();
		DoorClosed = true;
		DoorOpened = false;
	}
}

float UOpenDoorTest::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	//Find all overlapping actors
	TArray<AActor*> OverLappingActors;
	if(!PressurePlate) {return TotalMass;}
	PressurePlate->GetOverlappingActors(OUT OverLappingActors);

	for(AActor* Actor : OverLappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}
void UOpenDoorTest::FindPressurePlate() const
{
	if(!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has OpenDoor componoent but no prssure plate!"), *GetOwner()->GetName());
	}
}

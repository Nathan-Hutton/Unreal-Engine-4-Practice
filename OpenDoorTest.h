// Copyright Nathan Hutton 2022

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"

#include "OpenDoorTest.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UOpenDoorTest : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoorTest();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime); 
	float TotalMassOfActors() const;
	void FindAudioComponent();
	void FindPressurePlate() const;

private:

	float InitialYaw;
	float CurrentYaw;

	//tracks if sound has been played
	bool DoorOpened = false;
	bool DoorClosed = true;

	UPROPERTY(EditAnywhere)
	float DoorOpenMass = 50.f;

	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;
	
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 2.f;
	
	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = .75;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed =3.f;	

	float DoorLastOpened = 0.f;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;

};

// Code written by Steven Müller 2021


#include "OpenDoor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FRotator DoorRotation = GetOwner()->GetActorRotation(); // beinhaltet yaw, pitch, roll
	
	float CurrentYaw = GetOwner()->GetActorRotation().Yaw;	// aktueller startwert
	float EndYaw = -90.0f;									// angepeilter endwert
	float ExpoSpeed = 1.5f;									// bei Exponential Interpolation angabe etwa Meter/s

	DoorRotation.Yaw = FMath::FInterpTo(CurrentYaw, EndYaw, DeltaTime, ExpoSpeed);
	GetOwner()->SetActorRotation(DoorRotation);
}


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

	StartingYaw = GetOwner()->GetActorRotation().Yaw;
	HoldingYaw = StartingYaw;
	TargetYaw += StartingYaw;

	FindAudioComponent();

	if (!DoorTrigger)
	{
		UE_LOG(LogTemp, Error, TEXT("Object %s has no DoorTigger set !"), *GetOwner()->GetName());
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > MassToOpen) 
	{
		OpenDoor(DeltaTime);
		DoorLastOpen = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if ((GetWorld()->GetTimeSeconds() - DoorLastOpen) > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	HoldingYaw = FMath::FInterpTo(HoldingYaw, TargetYaw, DeltaTime, OpeningSpeed); // updateloop
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = HoldingYaw;
	GetOwner()->SetActorRotation(DoorRotation);
	CloseDoorPlayed = false;
	if (OpenDoorPlayed == false)
	{
		OpenDoorPlayed = true;
		DoorSound->Play();
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	HoldingYaw = FMath::FInterpTo(HoldingYaw, StartingYaw, DeltaTime, ClosingSpeed); // updateloop
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = HoldingYaw;
	GetOwner()->SetActorRotation(DoorRotation);
	OpenDoorPlayed = false;
	if (CloseDoorPlayed == false)
	{
		CloseDoorPlayed = true;
		DoorSound->Play();
	}
}

float UOpenDoor::TotalMassOfActors()
{
	float TotalMass = 0.0f;
	TArray<AActor*> OverlappingActors;
	DoorTrigger->GetOverlappingActors(OverlappingActors);
	if (!DoorTrigger) { return TotalMass; }
	
	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

void UOpenDoor::FindAudioComponent()
{
	DoorSound = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!DoorSound)
	{
		UE_LOG(LogTemp, Error, TEXT("Object %s has no AudioComponent attached !"), *GetOwner()->GetName());
	}
}


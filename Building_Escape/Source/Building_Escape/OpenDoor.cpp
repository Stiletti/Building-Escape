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

	if (!DoorTrigger)
	{
		UE_LOG(LogTemp, Error, TEXT("Object %s has no DoorTigger set !"), *GetOwner()->GetName());
	}
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (DoorTrigger && DoorTrigger->IsOverlappingActor(ActorThatOpens)) // 1. DoorTrigger für Crashprotection
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
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	HoldingYaw = FMath::FInterpTo(HoldingYaw, StartingYaw, DeltaTime, ClosingSpeed); // updateloop
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = HoldingYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}


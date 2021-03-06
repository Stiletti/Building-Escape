// Code written by Steven Müller 2021


#include "Grabber.h"
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetPlayerInput();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(SetTraceEnd());
	}
}

void UGrabber::Grab()
{
	UpdatePlayerViewPoint();
	LineTrace();
	ComponentToGrab = LineTrace().GetComponent();

	if (LineTrace().GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, SetTraceEnd());
		if (!PhysicsHandle) { return; }
	}
}

void UGrabber::Release()
{
	
	PhysicsHandle->ReleaseComponent();
	if (!PhysicsHandle) { return; }
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("NO PhysicHandle attached to %s"), *GetOwner()->GetName());
	}
	
}

void UGrabber::SetPlayerInput()
{
	Input = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!Input) { return; }
	if (Input)
	{
		UE_LOG(LogTemp, Warning, TEXT("InputComponent FOUND at %s"), *GetOwner()->GetName());
		Input->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		Input->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No InputComponent attached to %s"), *GetOwner()->GetName());
	}
}

void UGrabber::UpdatePlayerViewPoint()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerLocation, OUT PlayerRotation);
}

FHitResult UGrabber::LineTrace()
{
	UpdatePlayerViewPoint();
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerLocation,
		SetTraceEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	return Hit;
}

FVector UGrabber::SetTraceEnd()
{
	UpdatePlayerViewPoint();
	FVector LineTraceEnd = PlayerLocation + PlayerRotation.Vector() * Reach;
	return LineTraceEnd;
}


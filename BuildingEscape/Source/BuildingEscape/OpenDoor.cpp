// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Components/PrimitiveComponent.h"

#define OUT



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

	Owner = GetOwner();
	if (!Owner) {
		UE_LOG(LogTemp, Error, TEXT("The Door has not been found"));
	}
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("The PressurePlate hasn't been found in %s"), *GetOwner()->GetName());
	}
}

void UOpenDoor::OpenDoor()
{
	if (!Owner) return;
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}


void UOpenDoor::CloseDoor()
{
	if (!Owner) return;
	Owner->SetActorRotation(FRotator(0.0f, 0.f, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the trigger volume
	if (GetTotalMassOfActorOnPlate() > 50.f)/*TODO make it a parameter)*/
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	// Check if it's time to close the door
	if(GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
		CloseDoor();
} 

float UOpenDoor::GetTotalMassOfActorOnPlate() {
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	// Find all the overlapping actors
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	// Iterate over them summing up theri masses
	for (auto* Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName());
	}

	return TotalMass;
}

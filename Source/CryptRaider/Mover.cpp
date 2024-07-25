// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover.h"
#include "Math/Vector.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMover::UMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
 
// Called when the game starts
void UMover::BeginPlay()
{
	UE_LOG(LogTemp, Display, TEXT("UMover BeginPlay"));
	Super::BeginPlay();
	StartLocation = GetOwner()->GetActorLocation();
}

// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FVector currentLocation = GetOwner()->GetActorLocation();
	FVector targetLocation = StartLocation;

	if (ShouldMove)
	{
		targetLocation = StartLocation + MoveOffset;
	}

	Move(currentLocation, targetLocation,  DeltaTime);
} 

void UMover::Move(FVector currentLocation, FVector targetLocation,  float deltaTime)
{
	float speed = MoveOffset.Length() / MoveTime;

	FVector moveVector = FMath::VInterpConstantTo(currentLocation, targetLocation, deltaTime, speed);
	GetOwner()->SetActorLocation(moveVector);
}

void UMover::SetShouldMove(bool newSHouldMove)
{
	ShouldMove = newSHouldMove;
}
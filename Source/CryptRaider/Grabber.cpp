// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Math/Rotator.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

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
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle == nullptr)
	{
		return;
	}

	FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
	PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle)
	{
		FVector Start = GetComponentLocation();
		FVector End = Start + GetForwardVector() * MaxGrabDistance;
		FColor Color = FColor::Red;
		//DrawDebugLine(GetWorld(), Start, End, Color);
		//DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue);

		FHitResult HitResult;
		ECollisionChannel CollisionChannel = ECC_GameTraceChannel2;
		bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End,
		FQuat::Identity, 
		CollisionChannel,
		FCollisionShape::MakeSphere(GrabRadius));

		if (HasHit)
		{
			//DrawDebugSphere(GetWorld(), HitResult.Location, 10, 10, FColor::Red);
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Green);
		
			UPrimitiveComponent* HitComponent = HitResult.GetComponent();
			HitComponent->SetSimulatePhysics(true);
			HitComponent->WakeAllRigidBodies();
			//add grabbed tag to an hit Actor
			AActor* HitActor = HitResult.GetActor();
			HitActor->Tags.Add("Grabbed");
			HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			PhysicsHandle->GrabComponentAtLocationWithRotation(HitResult.GetComponent(),
																NAME_None,
																HitResult.ImpactPoint,
																GetComponentRotation());
		}
	}
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	
	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		AActor* grabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		grabbedActor->Tags.Remove("Grabbed");
		PhysicsHandle->ReleaseComponent();
	}
}

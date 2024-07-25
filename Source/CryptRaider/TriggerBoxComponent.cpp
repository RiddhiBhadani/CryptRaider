// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerBoxComponent.h"

// Sets default values for this component's properties
UTriggerBoxComponent::UTriggerBoxComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UTriggerBoxComponent::BeginPlay()
{
	UE_LOG(LogTemp, Display, TEXT("UTriggerBoxComponent BeginPlay"));
	Super::BeginPlay();
}

// Called every frame
void UTriggerBoxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
   Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

   AActor* unlockingActor = GetUnlockingActor();

   if (unlockingActor)
   {
		UPrimitiveComponent* primitiveComponent = Cast<UPrimitiveComponent>(unlockingActor->GetRootComponent());

		if (primitiveComponent)
		{
			primitiveComponent->SetSimulatePhysics(false);
		}
		unlockingActor->AttachToComponent(this,FAttachmentTransformRules::KeepWorldTransform);
		if (Mover != nullptr)
		{
			Mover->SetShouldMove(true);
		}
   }
   else
   {
		if (Mover != nullptr)
		{
			Mover->SetShouldMove(false);
		}
   }
}

void UTriggerBoxComponent::SetMover(UMover* newMover)
{
	Mover = newMover;
} 

AActor* UTriggerBoxComponent::GetUnlockingActor() const
{
	TArray<AActor*> OverlappingActorsArray;
	GetOverlappingActors(OverlappingActorsArray);

	if (OverlappingActorsArray.Num() > 0)
	{
			for(AActor* Actor :  OverlappingActorsArray)
			{
				bool HasAcceptableTag = Actor->ActorHasTag(ActorAcceptableTagName);
				bool IsGrabbed = Actor->ActorHasTag("Grabbed");
				if (HasAcceptableTag && !IsGrabbed)
				{
					return Actor;
				}
			}
	}
   return nullptr;
}
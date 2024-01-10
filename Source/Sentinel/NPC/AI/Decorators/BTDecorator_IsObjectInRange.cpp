// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/Decorators/BTDecorator_IsObjectInRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Sentinel/SentinelCharacter.h"

UBTDecorator_IsObjectInRange::UBTDecorator_IsObjectInRange()
{
	NodeName = TEXT("Is [OBJECT] in Range");
}

bool UBTDecorator_IsObjectInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if(!IsValid(OwnerComp.GetOwner())) return false;
	
	const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard == nullptr)
	{
		return false;
	}

	// Get the target pawn from the blackboard
	const auto bbTarget = Blackboard->GetValueAsObject(TargetKey.SelectedKeyName);
	if (!bbTarget) return false;

	const AActor* Target = Cast<AActor>(bbTarget);
	if (Target == nullptr || !Target->IsValidLowLevel())
	{
		return false;
	}

	// Check if the target actor is within range
	const FVector TargetLocation = Target->GetActorLocation();

	// Check if AI owner and its pawn are valid
	APawn* AIOwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (AIOwnerPawn == nullptr || !AIOwnerPawn->IsValidLowLevel())
	{
		return false;
	}

	const FVector AIOwnerLocation = AIOwnerPawn->GetActorLocation();
	const float DistanceSquared = FVector::DistSquared(TargetLocation, AIOwnerLocation);

	return DistanceSquared <= FMath::Square(Range);
}
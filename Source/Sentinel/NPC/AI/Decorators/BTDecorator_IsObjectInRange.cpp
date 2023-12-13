// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/Decorators/BTDecorator_IsObjectInRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsObjectInRange::UBTDecorator_IsObjectInRange()
{
	NodeName = TEXT("Is [OBJECT] in Range");
}

bool UBTDecorator_IsObjectInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	// Get the target pawn from the blackboard
	const AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
	if (Target == nullptr)
	{
		return false;
	}
	
	// Check if the target pawn is within range
	const float Distance = FVector::Dist(Target->GetActorLocation(), OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation());
	return Distance <= Range;
}

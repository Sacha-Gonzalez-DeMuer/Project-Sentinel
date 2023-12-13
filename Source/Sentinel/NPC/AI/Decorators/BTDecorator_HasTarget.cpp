// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/Decorators/BTDecorator_HasTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Sentinel/NPC/AI/BlackboardKeys.h"

UBTDecorator_HasTarget::UBTDecorator_HasTarget()
{
	NodeName = TEXT("Has Target");
}

bool UBTDecorator_HasTarget::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKeys::CurrentTarget) != nullptr;
}

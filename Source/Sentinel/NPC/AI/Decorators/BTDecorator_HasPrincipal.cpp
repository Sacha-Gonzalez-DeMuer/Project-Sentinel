// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/Decorators/BTDecorator_HasPrincipal.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Sentinel/NPC/AI/BlackboardKeys.h"

UBTDecorator_HasPrincipal::UBTDecorator_HasPrincipal()
{
	NodeName = TEXT("Has Principal");
}

bool UBTDecorator_HasPrincipal::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKeys::CurrentPrincipal) != nullptr;
}

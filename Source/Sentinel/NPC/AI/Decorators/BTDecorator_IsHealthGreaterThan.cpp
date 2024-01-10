// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/Decorators/BTDecorator_IsHealthGreaterThan.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/Components/HealthComponent.h"

UBTDecorator_IsHealthGreaterThan::UBTDecorator_IsHealthGreaterThan()
{
	NodeName = "Is Health Greater Than";
}

bool UBTDecorator_IsHealthGreaterThan::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                                                  uint8* NodeMemory) const
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if(ASentinelCharacter* Sentinel = Cast<ASentinelCharacter>(Blackboard->GetValueAsObject(SentinelToCheck.SelectedKeyName)))
	{
		return Sentinel->GetHealthComponent()->GetHealthInPercent() > GreaterThan;
	}

	
	
	return false;
}

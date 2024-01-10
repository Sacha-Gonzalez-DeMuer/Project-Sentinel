// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/Decorators/BTDecorator_IsToKillAlive.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/Components/HealthComponent.h"
#include "Sentinel/NPC/AI/BlackboardKeys.h"

UBTDecorator_IsToKillAlive::UBTDecorator_IsToKillAlive()
{
	NodeName = "Is Target To Kill Alive";
}

bool UBTDecorator_IsToKillAlive::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard == nullptr)
	{
		return false;
	}

	auto toKillBB = Blackboard->GetValueAsObject(FName(BBKeys::TargetToKill));
	if(!toKillBB) return false; // no one to kill

	if(ASentinelCharacter* ToKill = Cast<ASentinelCharacter>(Blackboard->GetValueAsObject(FName(BBKeys::TargetToKill))))
	{
		return ToKill->GetHealthComponent()->GetHealthInPercent() > 0;
	}

	
	return false;
}

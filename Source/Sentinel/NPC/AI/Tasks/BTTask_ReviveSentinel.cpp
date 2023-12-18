// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/Tasks/BTTask_ReviveSentinel.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/Components/HealthComponent.h"
#include "Sentinel/NPC/AI/BlackboardKeys.h"

UBTTask_ReviveSentinel::UBTTask_ReviveSentinel()
{
	NodeName = "Revive Sentinel";
}

EBTNodeResult::Type UBTTask_ReviveSentinel::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(ASentinelCharacter* Patient = Cast<ASentinelCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(SentinelToRevive.SelectedKeyName)))
	{
		ToRevive = Patient;
		if(ToRevive->IsOnLastStand())
		{
			ToRevive->GetHealthComponent()->Revive();
		}
	
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;;
}

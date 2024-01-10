// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/DirectorTasks/BTTask_CheckSentinelPressure.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Sentinel/NPC/GuardianSentinel.h"
#include "Sentinel/NPC/AI/BlackboardKeys.h"
#include "Sentinel/Actors/SentinelSquad.h"


UBTTask_CheckSentinelPressure::UBTTask_CheckSentinelPressure()
{
	NodeName = "Check Sentinel Pressure";
}

EBTNodeResult::Type UBTTask_CheckSentinelPressure::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	ASentinelSquad* Squad = Cast<ASentinelSquad>(Blackboard->GetValueAsObject(FName(BBKeys::Squad)));
	ASentinelCharacter* Sentinel = Cast<ASentinelCharacter>(Blackboard->GetValueAsObject(SentinelToCheck.SelectedKeyName));
	
	if(Squad && Sentinel)
	{
		if(Squad->CalculatePressure(Sentinel) > IsGreaterThan)
		{
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}

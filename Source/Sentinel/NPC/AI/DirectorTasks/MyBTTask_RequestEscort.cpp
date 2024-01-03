// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/DirectorTasks/MyBTTask_RequestEscort.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/Actors/SentinelSquad.h"
#include "Sentinel/NPC/AI/BlackboardKeys.h"

UMyBTTask_RequestEscort::UMyBTTask_RequestEscort()
{
	NodeName = "Request Escort";
}

EBTNodeResult::Type UMyBTTask_RequestEscort::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	ASentinelSquad* Squad = Cast<ASentinelSquad>(Blackboard->GetValueAsObject(FName(BBKeys::Squad)));
	ASentinelCharacter* ToEscort = Cast<ASentinelCharacter>(Blackboard->GetValueAsObject(SentinelToEscortKey.SelectedKeyName));
	if(Squad && ToEscort && Squad->RequestEscort(ToEscort))
	{
		return  EBTNodeResult::Succeeded;
	}

	return  EBTNodeResult::Failed;
}

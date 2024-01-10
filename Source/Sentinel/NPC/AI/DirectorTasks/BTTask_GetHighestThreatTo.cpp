// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/DirectorTasks/BTTask_GetHighestThreatTo.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/NPC/AI/SentinelController.h"

UBTTask_GetHighestThreatTo::UBTTask_GetHighestThreatTo()
{
	NodeName = "Get Highest Threat To";
}

EBTNodeResult::Type UBTTask_GetHighestThreatTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	ASentinelCharacter* Sentinel = Cast<ASentinelCharacter>(Blackboard->GetValueAsObject(GetThreatTo.SelectedKeyName));

	if(ASentinelController* SController = Sentinel->GetSentinelController())
	{
		ASentinelCharacter* HighestThreat = SController->GetHighestThreat();
		Blackboard->SetValueAsObject(SaveToKey.SelectedKeyName, HighestThreat);

		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}

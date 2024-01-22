// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/DirectorTasks/BTTask_RequestExplorer.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/Actors/SentinelSquad.h"
#include "Sentinel/NPC/AI/BlackboardKeys.h"
#include "Sentinel/NPC/AI/SentinelController.h"

UBTTask_RequestExplorer::UBTTask_RequestExplorer()
{
	NodeName = "Request Explorer";
}

EBTNodeResult::Type UBTTask_RequestExplorer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT(" request medic"))

	const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	const ASentinelSquad* Squad = Cast<ASentinelSquad>(Blackboard->GetValueAsObject(FName(BBKeys::SelfActor)));
	
	for(const ASentinelCharacter* Sentinel : Squad->GetSentinels())
	{
		if(const ASentinelController* SController = Sentinel->GetSentinelController())
		{
			const AActor* toExplore = Cast<AActor>(Blackboard->GetValueAsObject(TargetPosition.SelectedKeyName));
			if(toExplore)
			{
				const FVector toExplorePos = toExplore->GetActorLocation();
				if(SController->TrySetExplorer(toExplorePos))
				{
					return  EBTNodeResult::Succeeded;
				}
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Failed request Explorer :("))
	return  EBTNodeResult::Failed;
}

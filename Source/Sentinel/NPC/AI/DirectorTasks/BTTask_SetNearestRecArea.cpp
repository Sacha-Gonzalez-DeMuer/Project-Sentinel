// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/DirectorTasks/BTTask_SetNearestRecArea.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Sentinel/Actors/RecruitingArea.h"
#include "Sentinel/Actors/SentinelSquad.h"
#include "Sentinel/NPC/AI/BlackboardKeys.h"

UBTTask_SetNearestRecArea::UBTTask_SetNearestRecArea()
{
	NodeName = "Get Nearest RecruitArea";
}

EBTNodeResult::Type UBTTask_SetNearestRecArea::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const auto Blackboard = OwnerComp.GetBlackboardComponent();

	if(ASentinelSquad* Squad = Cast<ASentinelSquad>(Blackboard->GetValueAsObject(FName(BBKeys::SelfActor))))
	{
		auto SeenAreas = Squad->GetSeenRecruitAreas();
		if(SeenAreas.Num() <= 0)
		{
			return EBTNodeResult::Failed;
		}

		ARecruitingArea* ClosestArea = *SeenAreas.begin();
		float dist = FLT_MAX;
		
		for(ARecruitingArea* Area : SeenAreas)
		{
			float newdist = FVector::Dist(ClosestArea->GetActorLocation(), Area->GetActorLocation());
			if(newdist < dist)
			{
				ClosestArea = Area;
				dist = newdist;
			}
		}

		Blackboard->SetValueAsObject(SaveToKey.SelectedKeyName, ClosestArea);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

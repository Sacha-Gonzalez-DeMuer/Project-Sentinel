// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/AI/DirectorDecorators/BTDecorator_SeenRecruitables.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Sentinel/Actors/RecruitingArea.h"
#include "Sentinel/Actors/SentinelSquad.h"
#include "Sentinel/NPC/AI/BlackboardKeys.h"

UBTDecorator_SeenRecruitables::UBTDecorator_SeenRecruitables()
{
	NodeName = "Squad Saw Recruitables";
}

bool UBTDecorator_SeenRecruitables::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
                                                               uint8* NodeMemory) const
{
	const auto Blackboard = OwnerComp.GetBlackboardComponent();

	if(ASentinelSquad* Squad = Cast<ASentinelSquad>(Blackboard->GetValueAsObject(FName(BBKeys::SelfActor))))
	{
		auto Areas = Squad->GetSeenRecruitAreas();
		
		for(const ARecruitingArea* RecArea : Areas)
		{
			if(!RecArea->IsValidLowLevel()) continue;

			return true;
		}
	}

	return false;
}

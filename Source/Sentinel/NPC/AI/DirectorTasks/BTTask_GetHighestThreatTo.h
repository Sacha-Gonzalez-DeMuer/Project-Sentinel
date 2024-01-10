// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GetHighestThreatTo.generated.h"

/**
 * 
 */
UCLASS()
class SENTINEL_API UBTTask_GetHighestThreatTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()



public:
	UBTTask_GetHighestThreatTo();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector GetThreatTo; //sentinel to get highest threat from

	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector SaveToKey;

	
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_CheckSentinelPressure.generated.h"

/**
 * 
 */
UCLASS()
class SENTINEL_API UBTTask_CheckSentinelPressure : public UBTTask_BlackboardBase
{
	GENERATED_BODY()


public:
	UBTTask_CheckSentinelPressure();
		
	UPROPERTY(EditAnywhere, Category = "Settings")
	FBlackboardKeySelector SentinelToCheck; // sentinel to check for attackers

	UPROPERTY(EditAnywhere, Category = "Settings")
	float IsGreaterThan; //success if pressure on sentinel is higher than this value
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

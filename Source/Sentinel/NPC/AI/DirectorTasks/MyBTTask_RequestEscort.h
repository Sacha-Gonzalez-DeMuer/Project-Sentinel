// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MyBTTask_RequestEscort.generated.h"

/**
 * 
 */
UCLASS()
class SENTINEL_API UMyBTTask_RequestEscort : public UBTTask_BlackboardBase
{
	GENERATED_BODY()


public:
	UMyBTTask_RequestEscort();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector SentinelToEscortKey;
};

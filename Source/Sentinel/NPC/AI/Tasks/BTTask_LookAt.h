// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_LookAt.generated.h"

/**
 * 
 */
UCLASS()
class SENTINEL_API UBTTask_LookAt : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_LookAt();
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector LookatTarget;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

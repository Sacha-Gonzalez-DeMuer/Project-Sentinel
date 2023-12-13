// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_TakeCover.generated.h"

/**
 * 
 */
UCLASS()
class SENTINEL_API UBTTask_TakeCover : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_TakeCover();
	
	virtual void TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	
};

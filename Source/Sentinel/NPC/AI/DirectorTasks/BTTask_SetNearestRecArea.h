// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SetNearestRecArea.generated.h"

/**
 * 
 */
UCLASS()
class SENTINEL_API UBTTask_SetNearestRecArea : public UBTTask_BlackboardBase
{
	GENERATED_BODY()


public:
	UBTTask_SetNearestRecArea();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


	UPROPERTY(EditAnywhere, Category = "Settings")
	FBlackboardKeySelector SaveToKey; // sentinel to check for attackers
};

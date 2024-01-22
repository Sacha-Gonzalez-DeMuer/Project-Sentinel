// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RequestExplorer.generated.h"

/**
 * 
 */
UCLASS()
class SENTINEL_API UBTTask_RequestExplorer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()


public:
	UBTTask_RequestExplorer();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Settings")
	FBlackboardKeySelector TargetPosition; // sentinel to check for attackers

	UPROPERTY(EditAnywhere, Category = "Settings")
	int NumExplorers = 1;
	
};

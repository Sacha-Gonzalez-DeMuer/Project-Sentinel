// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SimpleAttack.generated.h"

/**
 * 
 */
UCLASS()
class SENTINEL_API UBTTask_SimpleAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	UBTTask_SimpleAttack();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere, Category="Settings")
	float Damage;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ReviveSentinel.generated.h"

class ASentinelCharacter;
/**
 * 
 */
UCLASS()
class SENTINEL_API UBTTask_ReviveSentinel : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_ReviveSentinel();

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector SentinelToRevive;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY()
	ASentinelCharacter* ToRevive;
};
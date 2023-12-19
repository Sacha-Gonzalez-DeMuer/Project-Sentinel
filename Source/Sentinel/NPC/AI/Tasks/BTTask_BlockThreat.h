// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Sentinel/SentinelCharacter.h"
#include "BTTask_BlockThreat.generated.h"

class ANPCBase;
class ASentinelCharacter;
/**
 * 
 */
UCLASS()
class SENTINEL_API UBTTask_BlockThreat : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_BlockThreat();

	// Distance in front of the principal that the NPC will try to reach
	UPROPERTY(EditAnywhere)
	float DistanceInFrontOfPrincipal;

	// Weight to control the influence of avoidance behavior based on threat proximity
	UPROPERTY(EditAnywhere)
	float AvoidanceWeight;

	UPROPERTY(EditAnywhere)
	float AcceptanceRadius;

	
	// The maximum distance at which the NPC will consider the threat for avoidance.
	UPROPERTY(EditAnywhere)
	float MaxAvoidanceDistance;
	
	// Minimum distance to consider a threat for avoidance behavior
	UPROPERTY(EditAnywhere)
	float MinDistanceToThreat;
	
	UPROPERTY(EditAnywhere)
	float MinDistanceToPrincipal;

	
private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory, float deltaSeconds) override;

	UPROPERTY()
	ANPCBase* NPC;
	
	UPROPERTY()
	ASentinelController* NPCController;

	UPROPERTY()
	ASentinelCharacter* Principal;

	UPROPERTY()
	UPawnMovementComponent* NPCMovement;

	UPROPERTY()
	ASentinelSquad* Squad;
};

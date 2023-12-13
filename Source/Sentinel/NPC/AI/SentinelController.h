// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SentinelController.generated.h"

class UBehaviorTree;
class UBehaviorTreeComponent;
class UBlackboardComponent;
class ANPCBase;
class ASentinelCharacter;

/**
 * Base controller class for all AI
 */
UCLASS()
class SENTINEL_API ASentinelController : public AAIController
{
	GENERATED_BODY()
	
public:
	ASentinelController(FObjectInitializer const& objectInit = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnSeePawn(APawn* SeenPawn);

	bool IsAttacking() const;
	void SetTarget(APawn* NewTarget) const;
	void UpdatePrincipal(ASentinelCharacter* NewPrincipal) const;
	void UpdateTarget(ASentinelCharacter* NewTarget) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAcces = "true"))
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAcces = "true"))
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAcces = "true"))
	TObjectPtr<UBlackboardComponent> BlackboardComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAcces = "true"))
	float RetargetingInterval;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAcces = "true"))
	float ThreatUpdateInterval;

	float GetThreatToTarget() const;
	float GetThreatToPrincipal() const;
	
protected:

	bool HasTarget() const;
	void AddThreat(ASentinelCharacter* NewThreat);
	void SetAttacking(bool Attacking);

	
	UPROPERTY()
	ANPCBase* NPCBase;

	UPROPERTY()
	ASentinelCharacter* Player;
	
	UPROPERTY()
	TSet<ASentinelCharacter*> SeenThreats;

	// How much of a threat is a certain character to the principal
	float EvaluateThreatToPrincipal(const ASentinelCharacter* Threat) const;
	
	// How much of a threat is this NPC to the NPC it's currently targeting
	float EvaluateThreatToTarget() const;

	void SetDefaultTarget() const;
private:
	bool bIsAttacking;
	
	float RetargetingIntervalTimer;
	float ThreatUpdateTimer;
	
	float ThreatToTarget;
	float ThreatToPrincipal;
	
	void InitializeBlackboardKeys();
	void RecalculateTargetPriority();
	void RecalculateThreatToTarget();
	void RecalculateThreatToPrincipal();

	ASentinelCharacter* GetPrincipal() const;
	ASentinelCharacter* GetTarget() const;
};

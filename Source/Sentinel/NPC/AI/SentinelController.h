// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/Actors/SentinelSquad.h"
#include "SentinelController.generated.h"

class UBehaviorTree;
class UBehaviorTreeComponent;
class UBlackboardComponent;
class ANPCBase;
class ASentinelCharacter;



UENUM(BlueprintType)
enum class ERoles : uint8
{
	Escort,
	Medic,
	Killer,
};



/**
 * Base controller class for all AI
 */
UCLASS()
class SENTINEL_API ASentinelController : public AAIController
{
	GENERATED_BODY()
	
public:
	ASentinelController(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnSeePawn(APawn* SeenPawn);

	bool IsAttacking() const;
	void SetTarget(ASentinelCharacter* NewTarget) const;
	void SetDefaultTarget();
	void SetPrincipal(ASentinelCharacter* NewPrincipal) const;

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

	float GetThreatToTarget();
	float GetThreatToPrincipal() const;
	FVector GetThreatLocation() const;
	void AddSeenThreat(ASentinelCharacter* NewThreat);

	UFUNCTION()
	void DisableBehaviorTree();

	UFUNCTION()
	void EnableBehaviorTree();

	UFUNCTION()
	void OnDeath();

	UFUNCTION()
	virtual void OnLastStand();
	
	ASentinelCharacter* GetPrincipal() const;
	ASentinelCharacter* GetTarget() const;
	TSet<ASentinelCharacter*> GetSeenThreats() const;


	void SetRole(ERoles toRole);
	
protected:

	bool HasTarget() const;
	ASentinelCharacter* GetCurrentThreat() const;
	
	void AddThreat(ASentinelCharacter* NewThreat);
	void SetAttacking(bool Attacking);

	void UpdateRetargetingTimer(float DeltaSeconds);
	void UpdateThreatToTargetTimer(float DeltaSeconds);

	
	UPROPERTY()
	ANPCBase* NPCBase;

	UPROPERTY()
	ASentinelCharacter* Player;
	
	UPROPERTY()
	TSet<ASentinelCharacter*> SeenThreats;

	
	// How much of a threat is this NPC to the NPC it's currently targeting
	float EvaluateThreatToTarget();

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
	float EvaluateThreatPriority(ASentinelCharacter* _SentinelCharacter);
	float EvaluateThreatToPrincipal(ASentinelCharacter* Threat);

	FVector CalculateProtectivePos(const ASentinelCharacter* Protectee, const ASentinelCharacter* Attacker, float DistanceInFrontOfProtectee);
	FVector CalculateSquadAvoidance(const ASentinelSquad* SquadToAvoid);
	FVector CalculateCharacterAvoidance(const ASentinelCharacter* ToAvoid);
};
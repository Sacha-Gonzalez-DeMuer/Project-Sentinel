// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/Actors/SentinelSquad.h"
#include "SentinelController.generated.h"

class UBlockThreat;
class UFollow;
class USteeringBehavior;
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
	void SetTargetToKill(ASentinelCharacter* Target) const;
	void SetToFollow();
	void SetToFollow(ASentinelCharacter* ToFollow);

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAcces = "true"))
	float MoveUpdateInterval;

	float GetThreatToTarget() const;
	float GetThreatToPrincipal() const;
	FVector GetThreatLocation() const;
	void AddSeenThreat(ASentinelCharacter* NewThreat);

	float GetProtectionToSentinel(ASentinelCharacter* Sentinel) const;
	float GetThreatToSentinel(ASentinelCharacter* Sentinel) const;
	
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
	ASentinelCharacter* GetTargetToKill() const;
	
	TSet<ASentinelCharacter*> GetSeenThreats() const;
	ASentinelCharacter* GetHighestThreat();
	

	void SetRole(ERoles toRole) const;
	ERoles GetRole() const;
	
	UFollow* GetFollowComponent() const;
	UBlockThreat* GetThreatBlockingComponent() const;

	bool TrySetEscort(ASentinelCharacter* ToEscort) const;
	bool TrySetKiller(ASentinelCharacter* Target) const;
	
protected:
	bool HasTarget() const;
	ASentinelCharacter* GetCurrentThreat() const;
	ASentinelCharacter* GetCurrentTarget() const;

	
	void AddThreat(ASentinelCharacter* NewThreat);
	void SetAttacking(bool Attacking);

	void UpdateRetargetingTimer(float DeltaSeconds);
	void UpdateThreatToTargetTimer(float DeltaSeconds);

	void UpdateMovement(float DeltaTime);
	
	UPROPERTY()
	ANPCBase* NPCBase;

	UPROPERTY()
	ASentinelCharacter* Player;
	
	UPROPERTY()
	TSet<ASentinelCharacter*> SeenThreats;

	UPROPERTY()
	USteeringBehavior* CurrentSteering;
	
	UPROPERTY(EditAnywhere)
	UFollow* FollowSteering;

	UPROPERTY(EditAnywhere)
	UBlockThreat* BlockThreatSteering;
	
	// How much of a threat is this NPC to the NPC it's currently targeting
	//float EvaluateThreatToTarget();
	void SetDefaultTarget() const;
private:
	bool bIsAttacking;
	
	float RetargetingIntervalTimer;
	float ThreatUpdateTimer;
	float MoveUpdateTimer;

	float ThreatToTarget;
	
	void InitializeBlackboardKeys();
	void RecalculateTargetPriority();
	void RecalculateThreatToTarget();
	float EvaluateThreatPriority(ASentinelCharacter* _SentinelCharacter);
	float EvaluateThreatToPrincipal(ASentinelCharacter* Threat);

	FVector CalculateProtectivePos(const ASentinelCharacter* Protectee, const ASentinelCharacter* Attacker, float DistanceInFrontOfProtectee) const;
	FVector CalculateSquadAvoidance(const ASentinelSquad* SquadToAvoid) const;
	FVector CalculateCharacterAvoidance(const ASentinelCharacter* ToAvoid) const;

	void SetBlockThreatSteering();
	void SetFollow(ASentinelCharacter* toFollow = nullptr);
};
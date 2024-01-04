// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SquadDirectorController.generated.h"


class ASentinelSquad;
class UBehaviorTree;
class UBehaviorTreeComponent;
class UBlackboardComponent;
class ASentinelCharacter;

/**
 * 
 */
UCLASS()
class SENTINEL_API ASquadDirectorController : public AAIController
{
	GENERATED_BODY()
public:
	ASquadDirectorController(FObjectInitializer const& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAcces = "true"))
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAcces = "true"))
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAcces = "true"))
	TObjectPtr<UBlackboardComponent> BlackboardComponent;

	void SetPrincipal(ASentinelCharacter* NewPrincipal);
private:
	UPROPERTY()
	ASentinelSquad* Squad;
	
	UPROPERTY(EditAnywhere)
	float UpdatePrincipalPressureInterval = 1.0f;


	float UpdatePrincipalPressureTimer = 0.1f;

	void InitializeBlackboardKeys();
	void UpdatePrincipalPressure(float DeltaSeconds);

	ASentinelCharacter* GetCurrentPrincipal() const;
};

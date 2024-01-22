// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SteeringBehavior.h"
#include "Components/ActorComponent.h"
#include "BlockThreat.generated.h"


class ASentinelSquad;
class UBlackboardComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SENTINEL_API UBlockThreat : public USteeringBehavior
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBlockThreat();

	void SetBlackboard(UBlackboardComponent* BlackboardComponent);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual FVector CalculateSteering(const ASentinelCharacter* SteeringAgent) override;

	UPROPERTY(EditAnywhere, Category= "Blocking")
	float DistanceInFrontOfPrincipal = 250.f;

	UPROPERTY(EditAnywhere, Category= "Blocking")
	float ArriveRadius = 50.f;

	UPROPERTY(EditAnywhere, Category="Avoidance")
	float ThreatAvoidanceRadius = 250.0f;

	UPROPERTY(EditAnywhere, Category="Avoidance")
	float ThreatAvoidanceWeight = .5f;

	UPROPERTY(EditAnywhere, Category="Avoidance")
	float AllyAvoidanceRadius = 250.0f;

	UPROPERTY(EditAnywhere, Category="Avoidance")
	float AllyAvoidanceWeight = .5f;

	FVector CalculateAvoidance(const ASentinelCharacter* SteeringAgent);
	bool IsBlocking(const ASentinelCharacter* SteeringAgent,  const ASentinelCharacter* Principal) const;


private:
	FVector Arrive(const ASentinelCharacter* SteeringAgent, const FVector& Target, float ArrivalRadius) const;

	bool BlockOccupied(const ASentinelCharacter* SteeringAgent, const ASentinelSquad* SentinelSquad) const;

	bool isBlocking;
};

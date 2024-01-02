// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SteeringBehavior.generated.h"

class ASentinelCharacter;
/**
 * 
 */
UCLASS()
class SENTINEL_API USteeringBehavior : public UActorComponent
{
	GENERATED_BODY()

public:
	USteeringBehavior();

	UFUNCTION()
	virtual FVector CalculateSteering(const ASentinelCharacter* SteeringAgent);
};

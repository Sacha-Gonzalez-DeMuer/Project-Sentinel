// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sentinel/NPC/AI/SentinelController.h"
#include "GuardianController.generated.h"

/**
 * 
 */
UCLASS()
class SENTINEL_API AGuardianController : public ASentinelController
{
	GENERATED_BODY()

public:
	AGuardianController(FObjectInitializer const& objectInit = FObjectInitializer::Get());

	virtual void OnSeePawn(APawn* SeenPawn) override;
	virtual void OnLastStand() override;

	virtual void Tick(float DeltaSeconds) override;
private:
	
};

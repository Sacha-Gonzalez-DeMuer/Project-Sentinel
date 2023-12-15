// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sentinel/NPC/AI/SentinelController.h"
#include "ZombController.generated.h"

/**
 * 
 */
UCLASS()
class SENTINEL_API AZombController : public ASentinelController
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;
};

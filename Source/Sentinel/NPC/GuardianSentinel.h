// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sentinel/NPC/NPCBase.h"
#include "GuardianSentinel.generated.h"

class AGuardianController;
/**
 * 
 */
UCLASS()
class SENTINEL_API AGuardianSentinel : public ANPCBase
{
	GENERATED_BODY()

public:
	AGuardianSentinel();

	UFUNCTION(BlueprintCallable)
	virtual void OnSeePawn(APawn* SeenPawn) override;
	
protected:
	virtual void BeginPlay() override;

	virtual void OnDeath() override;
	virtual void OnLastStand() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. Settings",
	meta = (AllowPrivateAccess = "true", ToolTip = "This collider stops any incoming enemy or projectile"))
	USphereComponent* ShieldCollider;

	
	UPROPERTY()
	AGuardianController* GuardianController;
};

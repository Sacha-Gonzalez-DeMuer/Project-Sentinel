// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPCBase.h"
#include "Sentinel/NPC/NPCBase.h"
#include "ZombSentinel.generated.h"

class AZombController;
/**
 * 
 */
UCLASS()
class SENTINEL_API AZombSentinel : public ANPCBase
{
	GENERATED_BODY()

public:
	AZombSentinel();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. Settings",
		meta = (AllowPrivateAccess = "true", ToolTip = "This collider will update the Sentinels target"))
	USphereComponent* RetargetCollision;

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void OnRetargetEnter(AActor* OtherActor);


	virtual void OnDeath() override;
	virtual void OnLastStand() override;
	
private:

	UPROPERTY()
	AZombController* ZombController;
};

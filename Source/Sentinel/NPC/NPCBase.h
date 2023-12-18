// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Sentinel/SentinelCharacter.h"
#include "NPCBase.generated.h"

class ASentinelController;
class UHealthComponent;
class USphereComponent;

UCLASS()
class SENTINEL_API ANPCBase : public ASentinelCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPCBase();
	
	bool IsPlayerTarget() const;
	bool IsPlayerPrincipal() const;

	virtual void Attack(const ASentinelCharacter* Target, float Damage) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI",  meta = (AllowPrivateAccess = "true"))
	bool PlayerIsTarget = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI",  meta = (AllowPrivateAccess = "true"))
	bool PlayerIsPrincipal = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI",
		meta = (ToolTip = "This is the default target for something."))
	AActor* DefaultTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UPawnSensingComponent* SensingComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. Settings", meta = (AllowPrivateAccess = "true"))
	USphereComponent* AttackCollider;

	UPROPERTY()
	ASentinelController* SentinelController;

public:	

	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	virtual void OnSeePawn(APawn* SeenPawn);
	

	UFUNCTION(BlueprintCallable)
	void OnAttackColliderEnter(AActor* OtherActor);


};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"
class ASentinelCharacter;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthEventDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthEventOPDelegate, int32, ParamName);







UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SENTINEL_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UPROPERTY(BlueprintCallable, Category = "1. Events")
	FHealthEventDelegate OnDeath;
	
	UPROPERTY(BlueprintCallable, Category = "1. Events")
	FHealthEventDelegate OnLastStand;

	UPROPERTY(BlueprintCallable, Category = "1. Events")
	FHealthEventDelegate OnRevive;

	UPROPERTY(BlueprintCallable, Category= "1. Events")
	FHealthEventOPDelegate OnTakeDamage;
	
	UFUNCTION(BlueprintCallable)
	virtual void TakeDamage(const int Amount, ASentinelCharacter* Instigator);
	
	UFUNCTION(BlueprintCallable)
	virtual void Heal(int Amount);

	UFUNCTION(BlueprintCallable)
	bool GetCanTakeDamage() const;
	
	void SetCanTakeDamage(bool CanThisTakeDamage);

	UFUNCTION(BlueprintCallable)
	int GetHealth() const;

	UFUNCTION(BlueprintCallable)
	int GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetHealthInPercent() const;

	UFUNCTION(BlueprintCallable)
	float GetLastStandTimerPercent() const;

	void SetParentCharacter(ASentinelCharacter* Parent);
	void SetCanBeRevived(bool _CanBeRevived);
	bool IsOnLastStand() const;
	bool IsDead() const;
	
	virtual void Revive();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void Die();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. Health", meta = (AllowPrivateAccess = "true"))
	int MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="1. Health", meta = (AllowPrivateAccess = "true"))
	float LastStandTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="1. Health", meta = (AllowPrivateAccess = "true"))
	float ReviveRechargeTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="1. Health", meta = (AllowPrivateAccess = "true"))
	float ReviveCooldownTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="1. Health", meta = (AllowPrivateAccess = "true"))
	bool CanBeRevived = true;
	
	int CurrentHealth;
	bool CanTakeDamage = true;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float ReviveCooldownTimer;
	float LastStandTimer;

	UPROPERTY()
	ASentinelCharacter* Character;

	bool _IsOnLastStand;
};

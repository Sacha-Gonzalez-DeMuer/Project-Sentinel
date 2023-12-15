// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathDelegate);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SENTINEL_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UPROPERTY(BlueprintCallable, Category = "1. Events")
	FOnDeathDelegate OnDeath;
	
	UFUNCTION(BlueprintCallable)
	virtual void TakeDamage(const int Amount);
	
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

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void Die();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. Health", meta = (AllowPrivateAccess = "true"))
	int MaxHealth = 100;
	
	int CurrentHealth;
	bool CanTakeDamage = true;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};

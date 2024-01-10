// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExperimentManager.generated.h"

class ASentinelCharacter;
class ASentinelDirector;
class ASentinelPlayerCharacter;

UCLASS()
class SENTINEL_API AExperimentManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExperimentManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (VisibleAnywhere = true))
	TSubclassOf<ASentinelDirector> Director;

	UFUNCTION(BlueprintCallable)
	void OnFinishEnter();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, meta = (VisibleAnywhere = true))
	ASentinelPlayerCharacter* Player;

	UFUNCTION()
	void OnPlayerTakeDamage(int Amount);

	void SaveData();
	void ResetWorld() const;
	
	int GetNrGuardiansInWorld() const;
	
	float TotalPlayerDamage;
	float CompletionTime;
	int SurvivingGuardians;
	int RevivedGuardians;
};

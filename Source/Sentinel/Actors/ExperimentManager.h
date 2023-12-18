// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExperimentManager.generated.h"

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
	TSubclassOf<ASentinelPlayerCharacter> Player;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (VisibleAnywhere = true))
	TSubclassOf<ASentinelDirector> Director;
	

	UFUNCTION(BlueprintCallable)
	void OnFinishEnter();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

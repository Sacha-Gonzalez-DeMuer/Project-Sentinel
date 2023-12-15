// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SentinelSquad.generated.h"

class ASentinelCharacter;

UCLASS()
class SENTINEL_API ASentinelSquad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASentinelSquad();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	int FactionIdx;

	UPROPERTY(EditAnywhere)
	int SquadIdx;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddSentinel(ASentinelCharacter* Sentinel);
	void SetIdx(int Faction, int Squad);
	
private:
	TArray<ASentinelCharacter*> Sentinels;
};

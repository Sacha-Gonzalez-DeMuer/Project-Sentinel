// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SentinelFaction.generated.h"

class ASentinelSquad;

UCLASS()
class SENTINEL_API ASentinelFaction : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASentinelFaction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASentinelSquad> SquadType;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetFactionIdx(int idx);
	int GetFactionIdx() const;

	ASentinelSquad* GetSquad(int SquadIdx);
	
private:
	int FactionIdx;

	TArray<ASentinelSquad*> Squads;
};

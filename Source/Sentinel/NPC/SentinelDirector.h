// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SentinelDirector.generated.h"


class ASentinelSquad;
class ASentinelCharacter;
class ASentinelFaction;

UCLASS(Blueprintable)
class SENTINEL_API ASentinelDirector : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASentinelDirector();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddSentinel(ASentinelCharacter* Sentinel);
	
	ASentinelFaction* CreateFaction(int FactionIdx);
	ASentinelSquad* GetSquad(int FactionIdx, int SquadIdx);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int MaxFactions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int MaxSquads;
	
	TArray<ASentinelFaction*> Factions;


};

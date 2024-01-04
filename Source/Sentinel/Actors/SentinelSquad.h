// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SentinelSquad.generated.h"

class ASentinelDirector;
class ASquadDirectorController;
class ASentinelFaction;
class ASentinelCharacter;

UCLASS()
class SENTINEL_API ASentinelSquad : public APawn
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

	UPROPERTY(EditAnywhere)
	ASentinelDirector* SentinelDirector;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddSentinel(ASentinelCharacter* Sentinel);
	void SetIdx(int NewFactionIdx, int NewSquadIdx);
	int GetSquadIdx() const;
	int GetFactionIdx() const;
	void SetFaction(ASentinelFaction* NewFaction);
	float GetAverageThreat() const;
	void LeaveSquad(ASentinelCharacter* SentinelToLeave);

	// retrieves how many squad members are attacking given target
	int GetNrAttackingSentinels(const ASentinelCharacter* Target) const;
	int GetNrAttackingSentinels(const ASentinelSquad* Target) const;
	int GetNrSentinels() const;
	TArray<ASentinelCharacter*> GetSentinels() const;
	TArray<ASentinelCharacter*> GetSeenThreats() const;

	void SetPrincipal(ASentinelCharacter* Principal);
	void RequestMedic(ASentinelCharacter* Patient);
	bool RequestEscort(ASentinelCharacter* ToEscort);

	float CalculatePressure(ASentinelCharacter* Principal) const;

	ASquadDirectorController* GetSquadDirector() const;

private:
	UPROPERTY(EditAnywhere)
	TArray<ASentinelCharacter*> Sentinels;

	UPROPERTY(EditAnywhere)
	ASentinelCharacter* DefaultPrincipal;

	UPROPERTY()
	ASentinelFaction* Faction;
};

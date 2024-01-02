// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SentinelCharacter.generated.h"

class ASentinelFaction;
class ASentinelSquad;
class ASentinelDirector;
class ASentinelController;
class UHealthComponent;
class UWidgetComponent;

UCLASS(Blueprintable)
class ASentinelCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASentinelCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	
	UHealthComponent* GetHealthComponent() const;
	int GetFactionIdx() const;
	int GetSquadIdx() const;
	
	bool IsAlly(int OtherFactionIdx) const;
	bool IsAlly(ASentinelCharacter* Sentinel) const;
	bool IsSquad(int OtherFactionIdx, int OtherSquadIdx) const;

	void SetFaction(int NewFactionIdx);
	void SetSquad(int NewSquadIdx);
	
	ASentinelController* GetSentinelController() const;
	ASentinelFaction* GetFaction() const;
	ASentinelSquad* GetSquad() const;
	ASentinelDirector* GetDirector() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsOnLastStand() const;

	virtual void Attack(const ASentinelCharacter* Target, float Damage);

	UFUNCTION()
	virtual void OnDeath();

	UFUNCTION()
	virtual void OnRevive();

	UFUNCTION()
	virtual void OnLastStand();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI",  meta = (AllowPrivateAccess = "true"),
	meta = (ToolTip = "The team this agent will be part of. A faction consists of one or multiple allied squads."))
	int FactionIdx;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI",  meta = (AllowPrivateAccess = "true"),
		meta = (ToolTip = "The group within the team that this agent will collaborate with."))
	int SquadIdx;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;

	// ANIMATIONS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage *LastStandMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage *RevivingMontage;
	
private:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ASentinelDirector* SentinelDirector;
};


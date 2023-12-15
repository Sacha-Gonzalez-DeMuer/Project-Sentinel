// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SentinelCharacter.generated.h"

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

	UHealthComponent* GetHealthComponent() const;
	int GetFaction() const;
	int GetSquad() const;

	void SetFaction(int NewFactionIdx);
	void SetSquad(int NewSquadIdx);

	ASentinelController* GetSentinelController() const;

	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI",  meta = (AllowPrivateAccess = "true"),
	meta = (ToolTip = "The team this agent will be part of. A faction consists of one or multiple allied squads."))
	int FactionIdx;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI",  meta = (AllowPrivateAccess = "true"),
		meta = (ToolTip = "The group within the team that this agent will collaborate with."))
	int SquadIdx;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;
};


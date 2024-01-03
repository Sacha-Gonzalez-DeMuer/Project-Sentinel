// Copyright Epic Games, Inc. All Rights Reserved.

#include "SentinelCharacter.h"

#include "Actors/SentinelSquad.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Components/HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "NPC/SentinelDirector.h"
#include "NPC/AI/SentinelController.h"

ASentinelCharacter::ASentinelCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// Create Health Component
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
	HealthComponent->SetParentCharacter(this);
}

void ASentinelCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ASentinelCharacter::BeginPlay()
{
	Super::BeginPlay();


	
	if (!SentinelDirector)
	{
		// Find the SentinelDirector in the level
		ASentinelDirector* FoundSentinelDirector = nullptr;
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASentinelDirector::StaticClass(), FoundActors);

		// Loop through the found actors to check if they match your criteria (if needed)
		for (AActor* Actor : FoundActors)
		{
			ASentinelDirector* Director = Cast<ASentinelDirector>(Actor);
			if (Director)
			{
				// Optionally, add additional criteria here if there are multiple directors in the level
				FoundSentinelDirector = Director;
				break; // Break out of the loop since we found the desired director
			}
		}

		if(FoundSentinelDirector)
		{
			SentinelDirector = FoundSentinelDirector;
		} else UE_LOG(LogTemp, Warning, TEXT("SentinelCharacter %s NOT ADDED to SentinelDirector"), *GetName());
	}
	else
	{
		// Add the sentinel to the director's list
		Cast<ASentinelDirector>(SentinelDirector)->AddSentinel(this);
	}
}

UHealthComponent* ASentinelCharacter::GetHealthComponent() const
{
	return HealthComponent;
}

int ASentinelCharacter::GetFactionIdx() const
{
	return FactionIdx;
}

int ASentinelCharacter::GetSquadIdx() const
{
	return SquadIdx;
}

bool ASentinelCharacter::IsAlly(int OtherFactionIdx) const
{
	return FactionIdx == OtherFactionIdx;
}

bool ASentinelCharacter:: IsAlly(ASentinelCharacter* Sentinel) const
{
	return Sentinel->GetFactionIdx() == FactionIdx;
}

bool ASentinelCharacter::IsSquad(int OtherFactionIdx, int OtherSquadIdx) const
{
	return OtherFactionIdx == FactionIdx && OtherSquadIdx == SquadIdx;
}

bool ASentinelCharacter::IsOnLastStand() const
{
	return HealthComponent->IsOnLastStand();
}

void ASentinelCharacter::Attack(const ASentinelCharacter* Target, float Damage)
{
	Target->GetHealthComponent()->TakeDamage(Damage, this);
}

void ASentinelCharacter::SetFaction(int NewFactionIdx)
{
	FactionIdx = NewFactionIdx;
}

void ASentinelCharacter::SetSquad(int NewSquadIdx)
{
	SquadIdx = NewSquadIdx;
}

ASentinelController* ASentinelCharacter::GetSentinelController() const
{
	if(!IsValidLowLevel())
	{
		return nullptr;
	}
	
	AController* AIController = GetController();
	if (AIController)
	{
		ASentinelController* SController = Cast<ASentinelController>(AIController);
		if (SController)
		{
			return SController;
		}
	}
	return nullptr;
}

ASentinelFaction* ASentinelCharacter::GetFaction() const
{
	return SentinelDirector->GetFaction(FactionIdx);
}

ASentinelSquad* ASentinelCharacter::GetSquad() const
{
	if(!IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("[ASentinelCharacter::GetSquad] FAILED TO GET SQUAD"));
		return nullptr;
	}

	if( !SentinelDirector)
	{
		UE_LOG(LogTemp, Error, TEXT("[ASentinelCharacter::GetSquad] DIRECTOR NOT LINKED TO %s"), *GetName());
		return nullptr;
	}
	return SentinelDirector->GetSquad(FactionIdx, SquadIdx);
}

ASentinelDirector* ASentinelCharacter::GetDirector() const
{
	return SentinelDirector;
}

void ASentinelCharacter::OnDeath()
{
	GetSquad()->LeaveSquad(this);
	GetSentinelController()->OnDeath();
}

void ASentinelCharacter::OnRevive()
{
	GetDirector()->AddSentinel(this);
	GetSentinelController()->OnLastStand();
}

void ASentinelCharacter::OnLastStand()
{
	PlayAnimMontage(LastStandMontage);
	GetSentinelController()->OnLastStand();
}

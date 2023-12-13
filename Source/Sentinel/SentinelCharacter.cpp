// Copyright Epic Games, Inc. All Rights Reserved.

#include "SentinelCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/HealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
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
	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
}

void ASentinelCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

UHealthComponent* ASentinelCharacter::GetHealthComponent() const
{
	return HealthComponent;
}

int ASentinelCharacter::GetFaction() const
{
	return FactionIdx;
}

int ASentinelCharacter::GetSquad() const
{
	return SquadIdx;
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
	if(ASentinelController* SController = Cast<ASentinelController>(GetController()))
	{
		return SController;
	}
	
	return nullptr;
}

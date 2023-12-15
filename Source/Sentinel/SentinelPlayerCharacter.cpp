// Fill out your copyright notice in the Description page of Project Settings.


#include "SentinelPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/HealthComponent.h"
#include "GameFramework/SpringArmComponent.h"

ASentinelPlayerCharacter::ASentinelPlayerCharacter()
{
	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
}

void ASentinelPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthComponent = GetComponentByClass<UHealthComponent>();
	if(!HealthComponent)
		UE_LOG(LogTemp, Error, TEXT("HealthComponent not linked in SentinelPlayerCharacter"));
}

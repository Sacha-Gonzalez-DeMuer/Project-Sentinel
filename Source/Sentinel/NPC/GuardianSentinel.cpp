// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/GuardianSentinel.h"

#include "AI/GuardianController.h"
#include "Components/SphereComponent.h"
#include "Sentinel/Components/HealthComponent.h"

AGuardianSentinel::AGuardianSentinel()
{
	ShieldCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Shield Collider"));
	ShieldCollider->InitSphereRadius(100.0f);
	ShieldCollider->SetRelativeLocation(FVector(100.0f, 10, 0));

	PlayerIsTarget = false;
	PlayerIsPrincipal = true;

}

void AGuardianSentinel::OnSeePawn(APawn* SeenPawn)
{
	if(GuardianController) GuardianController->OnSeePawn(SeenPawn);
	else
	{
		GuardianController = Cast<AGuardianController>(GetController());
		if(!GuardianController)
			UE_LOG(LogTemp, Error, TEXT("Controller not linked"));
	}
}

void AGuardianSentinel::BeginPlay()
{
	Super::BeginPlay();

	if(!HealthComponent)
		HealthComponent = GetComponentByClass<UHealthComponent>();
	
	//GuardianController = Cast<AGuardianController>(GetController());
}

void AGuardianSentinel::OnDeath()
{
	GuardianController->DisableBehaviorTree();
	Destroy();
}

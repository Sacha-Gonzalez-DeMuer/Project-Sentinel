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


	if(HealthComponent)
	{
		//HealthComponent->OnDeath.AddDynamic(this, &AGuardianSentinel::OnDeath);
		HealthComponent->OnLastStand.AddDynamic(this, &AGuardianSentinel::OnLastStand);
		HealthComponent->SetParentCharacter(this);
	}
	else UE_LOG(LogTemp, Warning, TEXT("ZombSentinel.cpp: NPCBase failed to setup healthcomponent"))

}

void AGuardianSentinel::OnDeath()
{
	GuardianController->DisableBehaviorTree();
	Destroy();
}

void AGuardianSentinel::OnLastStand()
{
	GetSquad()->RequestMedic(this);
	GuardianController->DisableBehaviorTree();
}

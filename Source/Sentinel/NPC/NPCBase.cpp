// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/NPC/NPCBase.h"

#include "AI/SentinelController.h"
#include "Components/SphereComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Sentinel/Components/HealthComponent.h"

// Sets default values
ANPCBase::ANPCBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Pawn Sensing Component
	SensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	SensingComponent->bOnlySensePlayers = false;

	AttackCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	AttackCollider->InitSphereRadius(100.0f);
	AttackCollider->BodyInstance.SetCollisionProfileName("Attack Collider");

	SentinelController = Cast<ASentinelController>(GetController());

}

bool ANPCBase::IsPlayerTarget() const
{
	return PlayerIsTarget;
}

bool ANPCBase::IsPlayerPrincipal() const
{
	return PlayerIsPrincipal;
}

void ANPCBase::Attack(const ASentinelCharacter* Target, float Damage)
{
	UE_LOG(LogTemp, Log, TEXT("Attacking target, Attacker %s"), *GetName());
	Target->GetHealthComponent()->TakeDamage(Damage, this);

	if(Target->IsOnLastStand()) GetSentinelController()->SetDefaultTarget();
}

// Called when the game starts or when spawned
void ANPCBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called to bind functionality to input
void ANPCBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ANPCBase::OnSeePawn(APawn* SeenPawn)
{
	if (SentinelController)
		SentinelController->OnSeePawn(SeenPawn);
}

void ANPCBase::OnAttackColliderEnter(AActor* OtherActor)
{
	if(!SentinelController->IsAttacking()) return;
	
	if(const ASentinelCharacter* Sentinel = Cast<ASentinelCharacter>(OtherActor))
		Sentinel->GetHealthComponent()->TakeDamage(10, this);
}

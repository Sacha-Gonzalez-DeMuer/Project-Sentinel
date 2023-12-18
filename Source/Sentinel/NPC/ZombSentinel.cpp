#include "Sentinel/NPC/ZombSentinel.h"

#include "GuardianSentinel.h"
#include "AI/SentinelController.h"
#include "AI/ZombController.h"
#include "Components/SphereComponent.h"
#include "Sentinel/Actors/SentinelSquad.h"
#include "Sentinel/Components/HealthComponent.h"


AZombSentinel::AZombSentinel()
{
	RetargetCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Retarget Collider"));
	PlayerIsTarget = true;
}
void AZombSentinel::BeginPlay()
{
	Super::BeginPlay();

	if(!SentinelController)
	{
		SentinelController = Cast<ASentinelController>(GetController());
		if(!SentinelController)
			UE_LOG(LogTemp, Error, TEXT("Controller not cached in ZombSentinel.cpp. Expect crashes."));
	}

	if(!HealthComponent)
	{
		HealthComponent = GetComponentByClass<UHealthComponent>();
	}

	if(HealthComponent)
	{
		//HealthComponent->OnDeath.AddDynamic(this, &AZombSentinel::OnDeath);
		HealthComponent->OnLastStand.AddDynamic(this, &AZombSentinel::OnLastStand);
		HealthComponent->SetParentCharacter(this);
	}
	else UE_LOG(LogTemp, Warning, TEXT("ZombSentinel.cpp: NPCBase failed to setup healthcomponent"))

	ZombController = Cast<AZombController>(GetController());
	if(!ZombController) UE_LOG(LogTemp, Error, TEXT("Failed to cache ZombController in ZombSentinel.cpp"));
	
}

void AZombSentinel::OnRetargetEnter(AActor* OtherActor)
{
	if (Cast<AZombSentinel>(OtherActor)) return;

	if (AGuardianSentinel* Guardian = Cast<AGuardianSentinel>(OtherActor))
	{
		ZombController->SetTarget(Guardian);
	}
}

void AZombSentinel::OnDeath()
{
	GetSquad()->LeaveSquad(this);
	ZombController->OnDeath();
	Destroy();
}

void AZombSentinel::OnLastStand()
{
	GetSquad()->RequestMedic(this);
	
	ZombController->OnLastStand();
}

#include "Sentinel/NPC/ZombSentinel.h"

#include "GuardianSentinel.h"
#include "AI/SentinelController.h"
#include "AI/ZombController.h"
#include "Components/SphereComponent.h"
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
		HealthComponent->OnDeath.AddDynamic(this, &AZombSentinel::OnDeath);
	else UE_LOG(LogTemp, Warning, TEXT("NPCBase failed to setup healthcomponent delegate"))

	ZombController = Cast<AZombController>(GetController());
	if(!ZombController) UE_LOG(LogTemp, Error, TEXT("Failed to cache ZombController in ZombSentinel.cpp"));
	
}

void AZombSentinel::OnRetargetEnter(AActor* OtherActor)
{
	if (Cast<AZombSentinel>(OtherActor)) return;

	if (AGuardianSentinel* Guardian = Cast<AGuardianSentinel>(OtherActor))
	{
		UE_LOG(LogTemp, Log, TEXT("Detected Guardian with name: %s"), *OtherActor->GetName());
		ZombController->SetTarget(Guardian);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Detected something with name: %s"), *OtherActor->GetName());
	}
}

void AZombSentinel::OnDeath()
{
	ZombController->OnDeath();
	Destroy();
}

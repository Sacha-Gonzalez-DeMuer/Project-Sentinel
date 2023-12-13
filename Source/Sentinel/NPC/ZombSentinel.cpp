#include "Sentinel/NPC/ZombSentinel.h"

#include "AI/SentinelController.h"
#include "Components/SphereComponent.h"

AZombSentinel::AZombSentinel()
{
	RetargetCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Retarget Collider"));
	RetargetCollision->InitSphereRadius(100.0f);
	RetargetCollision->SetRelativeLocation(FVector(100.0f, 10, 0));

	PlayerIsTarget = true;
}

void AZombSentinel::OnRetargetEnter(AActor* OtherActor)
{
	if(ASentinelCharacter* Sentinel = Cast<ASentinelCharacter>(OtherActor))
		SentinelController->SetTarget(Sentinel);
}
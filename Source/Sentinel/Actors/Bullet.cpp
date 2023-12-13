// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/Actors/Bullet.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Sentinel/SentinelCharacter.h"
#include "Sentinel/Components/HealthComponent.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	
	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 4.0f;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	Mesh->SetupAttachment(RootComponent);
}

void ABullet::Initialize(ASentinelCharacter* ShootingSentinel)
{
	Shooter = ShootingSentinel;
}

void ABullet::OnOverlapBegin(AActor* OtherActor)
{
	if(!Shooter) return;
	
	if(const ASentinelCharacter* HitCharacter = Cast<ASentinelCharacter>(OtherActor))
	{
		if(HitCharacter == Shooter) return;
		
		HitCharacter->GetHealthComponent()->TakeDamage(DamageToDeal);
		Destroy();
	}

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"


class ASentinelCharacter;
class UProjectileMovementComponent;
class USphereComponent;


UCLASS()
class SENTINEL_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

	void Initialize(ASentinelCharacter* ShootingSentinel);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. Settings", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. Settings", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. Settings", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "1. Settings", meta = (AllowPrivateAccess = "true"))
	float DamageToDeal;

	UFUNCTION(BlueprintCallable)
	virtual void DealDamage(AActor* OtherActor);

	UFUNCTION(BlueprintCallable)
	virtual void Heal(AActor* OtherActor);
	
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
    
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	ASentinelCharacter* Shooter;
};

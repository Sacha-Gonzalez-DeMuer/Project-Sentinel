// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/Components/HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
	: CurrentHealth(MaxHealth)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UHealthComponent::TakeDamage(const int Amount)
{
	if (const AActor* OwnerActor = GetOwner())
	{
		const FString ActorName = OwnerActor->GetName();
		UE_LOG(LogTemp, Log, TEXT("Actor %s took damage: %d"), *ActorName, Amount);
	}
	

	if (!CanTakeDamage)
		return;
	
	CurrentHealth -= Amount;
	
	if (CurrentHealth <= 0)
		Die();
}

void UHealthComponent::Heal(int Amount)
{
	CurrentHealth += Amount;
	
	if(CurrentHealth > MaxHealth)
		CurrentHealth = MaxHealth;
}

bool UHealthComponent::GetCanTakeDamage() const
{
	return CanTakeDamage;
}

void UHealthComponent::SetCanTakeDamage(bool CanThisTakeDamage)
{
	CanTakeDamage = CanThisTakeDamage;
}

int UHealthComponent::GetHealth() const
{
	return CurrentHealth;
}

int UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float UHealthComponent::GetHealthInPercent() const
{
	return static_cast<float>(CurrentHealth) / MaxHealth;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UHealthComponent::Die()
{
	UE_LOG(LogTemp, Log, TEXT("deded x.x"));
	OnDeath.Broadcast();
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


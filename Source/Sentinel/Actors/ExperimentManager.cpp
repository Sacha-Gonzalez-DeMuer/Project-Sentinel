// Fill out your copyright notice in the Description page of Project Settings.


#include "Sentinel/Actors/ExperimentManager.h"

#include "Kismet/GameplayStatics.h"
#include "Sentinel/SentinelPlayerCharacter.h"
#include "Sentinel/Components/HealthComponent.h"
#include "Sentinel/NPC/GuardianSentinel.h"

// Sets default values
AExperimentManager::AExperimentManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AExperimentManager::BeginPlay()
{
	Super::BeginPlay();

	if (!Player)
	{
		// Attempt to find the Player in the scene if it's not set
		Player = Cast<ASentinelPlayerCharacter>( UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (!Player)
		{
			UE_LOG(LogTemp, Error, TEXT("Player not set or found in Experiment Manager!!"));
			return;
		}
	}


	//Player->GetHealthComponent()->OnTakeDamage.AddDynamic(this, &AExperimentManager::OnPlayerTakeDamage);
	//Player->GetHealthComponent()->OnDeath.AddDynamic(this,  &AExperimentManager::OnFinishEnter);
}

void AExperimentManager::OnFinishEnter()
{
	// stop collecting data, save it, reset
	SaveData();
	ResetWorld();
}

// Called every frame
void AExperimentManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExperimentManager::OnPlayerTakeDamage(int Amount)
{
	TotalPlayerDamage += Amount;
}

void AExperimentManager::SaveData()
{
	// Get the game's save directory
	const FString SaveDirectory = FPaths::ProjectSavedDir();

	// Get the current date and time to create a unique file name
	const FDateTime CurrentDateTime = FDateTime::Now();
	const FString DateTimeString = CurrentDateTime.ToString(TEXT("%Y%m%d%H%M%S"));
	
	// Create a unique file name with the current date and time
	const FString FileName = FString::Printf(TEXT("ExperimentData_%s.txt"), *DateTimeString);
	const FString AbsoluteFilePath = SaveDirectory + FileName;


	const int NrSurvivingGuardians = GetNrGuardiansInWorld();
	
	// Convert the data to a string
	const FString DataString = FString::Printf(TEXT(
		"TotalPlayerDamage: %f \n "
		"CompletionTime: %f \n"
		"Nr Surviving Guardians: %d"),
		TotalPlayerDamage, CompletionTime, NrSurvivingGuardians);

	// Save the data to a text file
	if (FFileHelper::SaveStringToFile(DataString, *AbsoluteFilePath))
	{
		UE_LOG(LogTemp, Warning, TEXT("[AExperimentManager::SaveData] Data saved to file: %s"), *AbsoluteFilePath);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[AExperimentManager::SaveData] Failed to save data to file!"));
	}

	// Reset the data for the next experiment
	TotalPlayerDamage = 0;
}

void AExperimentManager::ResetWorld() const
{
	// Get the current level name
	const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	// Open the current level to effectively reload it
	UGameplayStatics::OpenLevel(this, FName(*CurrentLevelName));
}

int AExperimentManager::GetNrGuardiansInWorld() const
{
	// Get all instances of SentinelGuardian in the world
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGuardianSentinel::StaticClass(), FoundActors);

	// Count the number of surviving SentinelGuardians
	int NrSurvivingGuardians = 0;
	for (AActor* Actor : FoundActors)
	{
		const AGuardianSentinel* Guardian = Cast<AGuardianSentinel>(Actor);
		if (Guardian && !Guardian->GetHealthComponent()->IsDead()) // Assuming IsDead() is a function that indicates whether the guardian is still alive
		{
			NrSurvivingGuardians++;
		}
	}

	return NrSurvivingGuardians;
}


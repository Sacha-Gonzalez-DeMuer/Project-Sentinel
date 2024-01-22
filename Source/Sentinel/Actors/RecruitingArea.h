// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RecruitingArea.generated.h"

class ASentinelPlayerCharacter;
class UBoxComponent;
class ASentinelCharacter;

UCLASS()
class SENTINEL_API ARecruitingArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARecruitingArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	bool TriggerBySentinelOnly;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
	UBoxComponent* TriggerArea;

	UPROPERTY(EditAnywhere)
	TArray<ASentinelCharacter*> Sentinels;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> ToRemove;

	UPROPERTY(EditAnywhere)
	float TimeToRecruit = 10.0f;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnOverlapEnd(AActor* OtherActor);

	void OverlapTick(AActor* OtherActor, float DeltaTime);

	UFUNCTION(BlueprintCallable)
	float GetRecruitProgress() const;
	
	UPROPERTY()
	ASentinelPlayerCharacter* Player;

	bool BeingRecruited = false;
	float RecruitTimer = 0.0f;

	void Recruit();

	TArray<ASentinelCharacter*> RecruitingSentinels;
};

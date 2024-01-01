// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RecruitingArea.generated.h"

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
	UBoxComponent* TriggerArea;

	UPROPERTY(EditAnywhere)
	TArray<ASentinelCharacter*> Sentinels;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> ToRemove;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	UFUNCTION(BlueprintCallable)
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};

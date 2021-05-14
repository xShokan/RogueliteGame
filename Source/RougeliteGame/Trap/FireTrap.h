// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <edevdefs.h>

#include "TrapBase.h"
#include "FireTrap.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELITEGAME_API AFireTrap : public ATrapBase
{
	GENERATED_BODY()

	AFireTrap();

	virtual void BeginPlay() override;

	virtual  void Tick(float DeltaSeconds) override;
public:
	UFUNCTION()
	void ParticleFinished(class UParticleSystemComponent* PSystem);

	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	void PlayFire();

	void BurningDamage();
	
	UPROPERTY(VisibleDefaultsOnly)
	class UParticleSystem* FireParticle;

	UPROPERTY(VisibleDefaultsOnly)
	class UParticleSystemComponent* ParticleSystemComponent;

	bool bParticleFinished;

	FTimerHandle DamageTimerHandle;

	FTimerHandle BurningTimerHandle;

	UPROPERTY(EditAnywhere)
	float StartTime;
	
	UPROPERTY(EditAnywhere)
	float CycleTime;

	UPROPERTY(EditAnywhere)
	int32 DamageTimes;
};
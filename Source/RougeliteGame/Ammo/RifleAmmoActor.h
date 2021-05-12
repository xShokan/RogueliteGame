// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AmmoBaseActor.h"
#include "RifleAmmoActor.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELITEGAME_API ARifleAmmoActor : public AAmmoBaseActor
{
	GENERATED_BODY()

	ARifleAmmoActor();

	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleDefaultsOnly)
	class UBoxComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly)
	class UProjectileMovementComponent* MovementComponent;

	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void ExplodeBySelf() override;
};

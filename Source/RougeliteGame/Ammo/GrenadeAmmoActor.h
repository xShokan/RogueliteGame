// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AmmoBaseActor.h"
#include "GrenadeAmmoActor.generated.h"

/**
 * 
 */
UCLASS()
class ROUGELITEGAME_API AGrenadeAmmoActor : public AAmmoBaseActor
{
	GENERATED_BODY()
	
	AGrenadeAmmoActor();

	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	UPROPERTY(VisibleDefaultsOnly)
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly)
	class UProjectileMovementComponent* ProjectileMovement;
	
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void ExplodeBySelf() override;
};

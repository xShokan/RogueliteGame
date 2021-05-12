// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "AmmoBaseActor.generated.h"

UCLASS()
class ROUGELITEGAME_API AAmmoBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmoBaseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly)
	class UStaticMeshComponent* AmmoMeshComponent;

	UPROPERTY(VisibleDefaultsOnly)
	class UParticleSystem* AmmoExplosionParticle;

	UPROPERTY(VisibleDefaultsOnly)
	class USoundBase* AmmoExplosionSound;

	UPROPERTY(VisibleDefaultsOnly)
	float Damage;

	FTimerHandle AmmoTimerHandle;

	UFUNCTION()
    virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit){};

	UFUNCTION()
    virtual void ExplodeBySelf(){};

	void SpawnFloatingDamage();
};

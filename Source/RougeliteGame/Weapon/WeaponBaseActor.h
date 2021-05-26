// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "WeaponBaseActor.generated.h"

UCLASS()
class ROUGELITEGAME_API AWeaponBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBaseActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent *WeaponMeshComponent;

	FActorSpawnParameters SpawnParameters;
	FTransform AmmoTransform;

	UPROPERTY(VisibleDefaultsOnly)
	class UParticleSystem* WeaponShotParticle;

	UPROPERTY(VisibleDefaultsOnly)
	class USoundBase* WeaponShotSound;

	UPROPERTY(EditAnywhere)
	FString Name;
	
	UPROPERTY(EditAnywhere)
	int32 AmmoNumInClip;

	UPROPERTY(EditAnywhere)
	int32 AmmoTotalNum;

	UPROPERTY(VisibleDefaultsOnly)
	int32 ClipMaxAmmo;

	UPROPERTY(VisibleDefaultsOnly)
	int32 WeaponMaxAmmo;

	UPROPERTY(VisibleDefaultsOnly)
	class AFirstPersonCharacter* Character;
	
	virtual void Fire();

	virtual void Reload();

	virtual void FillAmmo();

	UFUNCTION(NetMulticast, Unreliable)
	void PlayEmitterSoundMulticast();

};

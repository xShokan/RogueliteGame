// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeGunActor.h"


#include "ConstructorHelpers.h"
#include "Ammo/GrenadeAmmoActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "ParticleDefinitions.h"
#include "Engine/Engine.h"
#include "Sound/SoundBase.h"

AGrenadeGunActor::AGrenadeGunActor()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponSkeletalMesh(TEXT("SkeletalMesh'/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (WeaponSkeletalMesh.Succeeded())
	{
		WeaponMeshComponent->SetSkeletalMesh(WeaponSkeletalMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ShotParticle(TEXT("ParticleSystem'/Game/Effects/SciFiWeapLight/FX/Particles/P_SniperRifle_MuzzleFlash_Dark.P_SniperRifle_MuzzleFlash_Dark'"));
	if (ShotParticle.Succeeded())
	{
		WeaponShotParticle = ShotParticle.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> ShotSound(TEXT("SoundWave'/Game/Effects/SciFiWeapLight/Sound/SniperRifle/Wavs/SR_Fire03.SR_Fire03'"));
	if (ShotSound.Succeeded())
	{
		WeaponShotSound = ShotSound.Object;
	}

	Name = TEXT("Grenade");
	ClipMaxAmmo = 5;
	WeaponMaxAmmo = 30;
	AmmoNumInClip = ClipMaxAmmo;
	AmmoTotalNum = WeaponMaxAmmo;
}

void AGrenadeGunActor::Fire()
{
	Super::Fire();

	if (AmmoNumInClip > 0)
	{
		SpawnParameters.bNoFail = true;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		AmmoTransform.SetLocation(WeaponMeshComponent->GetSocketLocation(TEXT("Muzzle")));
		AmmoTransform.SetRotation(FQuat(WeaponMeshComponent->GetSocketRotation(TEXT("Muzzle"))));

		AGrenadeAmmoActor* AmmoActor = GetWorld()->SpawnActor<AGrenadeAmmoActor>(AGrenadeAmmoActor::StaticClass(), AmmoTransform, SpawnParameters);
		AmmoActor->Instigator = this->GetInstigator();

		AmmoNumInClip--;
	}
}

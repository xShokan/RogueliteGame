// Fill out your copyright notice in the Description page of Project Settings.


#include "RifleGunActor.h"


#include "AI/AICharacter.h"
#include "ConstructorHelpers.h"
#include "Ammo/RifleAmmoActor.h"
#include "AI/HumanAIController.h"
#include "FirstPersonCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "ParticleDefinitions.h"
#include "Camera/CameraComponent.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundBase.h"

ARifleGunActor::ARifleGunActor()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponSkeletalMesh(TEXT("SkeletalMesh'/Game/Weapon/MilitaryWeapSilver/Weapons/Assault_Rifle_A.Assault_Rifle_A'"));
	if (WeaponSkeletalMesh.Succeeded())
	{
		WeaponMeshComponent->SetSkeletalMesh(WeaponSkeletalMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ShotParticle(TEXT("ParticleSystem'/Game/Weapon/MilitaryWeapSilver/FX/P_AssaultRifle_MuzzleFlash.P_AssaultRifle_MuzzleFlash'"));
	if (ShotParticle.Succeeded())
	{
		WeaponShotParticle = ShotParticle.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> ShotSound(TEXT("SoundWave'/Game/Weapon/MilitaryWeapSilver/Sound/Rifle/Wavs/RifleA_Fire01.RifleA_Fire01'"));
	if (ShotSound.Succeeded())
	{
		WeaponShotSound = ShotSound.Object;
	}

	Name = TEXT("Rifle");
	ClipMaxAmmo = 30;
	WeaponMaxAmmo = 100;
	AmmoNumInClip = ClipMaxAmmo;
	AmmoTotalNum = WeaponMaxAmmo;
}

void ARifleGunActor::Fire()
{
	Super::Fire();
	
	if (AmmoNumInClip > 0)
	{
		SpawnParameters.bNoFail = true;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		
		AmmoTransform.SetLocation(WeaponMeshComponent->GetSocketLocation(TEXT("Muzzle")));
		if (Cast<AFirstPersonCharacter>(Instigator))
		{
			AmmoTransform.SetRotation(FQuat(WeaponMeshComponent->GetSocketRotation(TEXT("Muzzle"))));
		}
		else if (Cast<AAICharacter>(Instigator))
		{
			AmmoTransform.SetRotation(FQuat(Instigator->GetActorRotation()));
		}

		FVector LineStart = AmmoTransform.GetLocation();
		AFirstPersonCharacter* MyFirstPersonCharacter = Cast<AFirstPersonCharacter>(Instigator);
		if (MyFirstPersonCharacter)
		{
			FVector LineEnd = LineStart + 5000.0f * MyFirstPersonCharacter->FirstPersonCameraComponent->GetComponentRotation().Vector();
			AmmoTransform.SetRotation(FQuat(UKismetMathLibrary::FindLookAtRotation(LineStart, LineEnd)));
		}
		
		ARifleAmmoActor* AmmoActor = GetWorld()->SpawnActor<ARifleAmmoActor>(ARifleAmmoActor::StaticClass(), AmmoTransform, SpawnParameters);
		AmmoActor->Instigator = this->GetInstigator();

		AmmoNumInClip--;
	}
}
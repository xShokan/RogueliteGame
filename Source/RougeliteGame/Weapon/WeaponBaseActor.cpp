// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBaseActor.h"

#include "FirstPersonCharacter.h"
#include "TimerManager.h"
#include "UnrealNetwork.h"
#include "Animation/AnimInstance.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AWeaponBaseActor::AWeaponBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	// WeaponMeshComponent->SetupAttachment(RootComponent);
	RootComponent = WeaponMeshComponent;
	WeaponMeshComponent->bCastDynamicShadow = false;
	WeaponMeshComponent->CastShadow = false;

	bReplicates = true;
}

// Called when the game starts or when spawned
void AWeaponBaseActor::BeginPlay()
{
	Super::BeginPlay();

} 

// Called every frame
void AWeaponBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBaseActor::Fire()
{
	/*Character = Cast<AFirstPersonCharacter>(GetInstigator());
	if (AmmoNumInClip > 0)
	{
		if (Character && Character->FireAnimation)
		{
			CharacterAnimInstance = Character->GetMesh()->GetAnimInstance();
			if (CharacterAnimInstance && WeaponShotParticle && WeaponShotSound)
			{
				// AnimInstance->Montage_Play(Character->FireAnimation);
				PlayMontageEmitterSound(CharacterAnimInstance, Character->FireAnimation);
			}
		}
	}*/
	PlayEmitterSoundMulticast();
}

void AWeaponBaseActor::Reload()
{
}

void AWeaponBaseActor::FillAmmo()
{
	AmmoTotalNum = WeaponMaxAmmo;
}

void AWeaponBaseActor::PlayEmitterSoundMulticast_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), WeaponShotParticle, WeaponMeshComponent->GetSocketLocation(TEXT("Muzzle")), WeaponMeshComponent->GetSocketRotation(TEXT("Muzzle")));
	UGameplayStatics::PlaySoundAtLocation(this, WeaponShotSound, WeaponMeshComponent->GetSocketLocation(TEXT("Muzzle")), WeaponMeshComponent->GetSocketRotation(TEXT("Muzzle")));
}

void AWeaponBaseActor::ReloadAmmo()
{
	
}

void AWeaponBaseActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeaponBaseActor, AmmoNumInClip);
}

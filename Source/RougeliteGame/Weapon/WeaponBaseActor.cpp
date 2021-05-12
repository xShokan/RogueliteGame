// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBaseActor.h"

#include "FirstPersonCharacter.h"
#include "TimerManager.h"
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
	PrimaryActorTick.bCanEverTick = true;

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	// WeaponMeshComponent->SetupAttachment(RootComponent);
	RootComponent = WeaponMeshComponent;
	WeaponMeshComponent->bCastDynamicShadow = false;
	WeaponMeshComponent->CastShadow = false;
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
	Character = Cast<AFirstPersonCharacter>(GetInstigator());
	if (AmmoNumInClip > 0)
	{
		if (Character && Character->FireAnimation && !Character->bAim)
		{
			UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->Montage_Play(Character->FireAnimation);
			}
		}
	}
}

void AWeaponBaseActor::Reload()
{
	if (AmmoTotalNum > 0)
	{
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
			if (AmmoTotalNum < ClipMaxAmmo - AmmoNumInClip)
			{
				AmmoNumInClip += AmmoTotalNum;
				AmmoTotalNum = 0;
			}
			else
			{
				AmmoTotalNum -= ClipMaxAmmo - AmmoNumInClip;
				AmmoNumInClip = ClipMaxAmmo;
			}
        }, 2.0f, false);
	}
	
}

void AWeaponBaseActor::FillAmmo()
{
	AmmoTotalNum = WeaponMaxAmmo;
}

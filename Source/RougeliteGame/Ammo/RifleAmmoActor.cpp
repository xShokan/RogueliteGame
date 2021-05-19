// Fill out your copyright notice in the Description page of Project Settings.


#include "RifleAmmoActor.h"



#include "AI/AICharacter.h"
#include "ConstructorHelpers.h"
#include "Widget/DamageWidget.h"
#include "Widget/FloatingDamage.h"
#include "AI/HumanAIController.h"
#include "FirstPersonCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/MovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "ParticleDefinitions.h"
#include "TextBlock.h"
#include "WidgetComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

ARifleAmmoActor::ARifleAmmoActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Use a sphere as a simple collision representation
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &ARifleAmmoActor::OnHit);   // set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComponent->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComponent;
	
	// Use a ProjectileMovementComponent to govern this projectile's movement
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = CollisionComponent;
	MovementComponent->InitialSpeed = 10000.0f;
	MovementComponent->MaxSpeed = 100000.f;
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bShouldBounce = false;
	
	// Set static mesh
	AmmoMeshComponent->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> AmmoMesh(TEXT("StaticMesh'/Game/Weapon/MilitaryWeapSilver/Weapons/AssaultRifleA_Ammo.AssaultRifleA_Ammo'"));
	if (AmmoMesh.Succeeded())
	{
		AmmoMeshComponent->SetStaticMesh(AmmoMesh.Object);
        AmmoMeshComponent->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));
	}

	CollisionComponent->InitBoxExtent(AmmoMeshComponent->GetStaticMesh()->GetBounds().BoxExtent);
	
	// Die after 3 seconds by default
	// InitialLifeSpan = 3.0f;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionParticle(TEXT("ParticleSystem'/Game/Weapon/MilitaryWeapSilver/FX/P_Impact_Metal_Small_01.P_Impact_Metal_Small_01'"));
	if (ExplosionParticle.Succeeded())
	{
		AmmoExplosionParticle = ExplosionParticle.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> ExplosionSound(TEXT("SoundWave'/Game/Weapon/MilitaryWeapSilver/Sound/Rifle/Wavs/Rifle_ImpactBody01.Rifle_ImpactBody01'"));
	if (ExplosionSound.Succeeded())
	{
		AmmoExplosionSound = ExplosionSound.Object;
	}

	Damage = 5.0f;

	DamageTime = 2.0f;
}

void ARifleAmmoActor::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(AmmoTimerHandle, this, &AAmmoBaseActor::ExplodeBySelf, 3.0f);
}

void ARifleAmmoActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AmmoExplosionParticle, GetActorLocation(), GetActorRotation());
		UGameplayStatics::PlaySoundAtLocation(this, AmmoExplosionSound, GetActorLocation());
		AAICharacter* DamageAICharacter = Cast<AAICharacter>(OtherActor);
		// Hit AI
		if (DamageAICharacter && DamageAICharacter->CurrentHealth > 0.1f && Cast<AFirstPersonCharacter>(Instigator->Instigator))
		{
			if (Hit.Location.Z > DamageAICharacter->HeadLowBoundZ)
			{
				UGameplayStatics::ApplyDamage(DamageAICharacter, DamageTime * Damage, nullptr, GetInstigator()->GetInstigator(), nullptr);
				SpawnFloatingDamage(DamageTime * Damage);
			}
			else
			{
				UGameplayStatics::ApplyDamage(DamageAICharacter, Damage, nullptr, GetInstigator()->GetInstigator(), nullptr);
				SpawnFloatingDamage(Damage);
			}
			Destroy();
		}

		AFirstPersonCharacter* DamageMyFirstPersonCharacter = Cast<AFirstPersonCharacter>(OtherActor);
		// Hit Person
		if (DamageMyFirstPersonCharacter && DamageMyFirstPersonCharacter->CurrentHealth > 0.1f)
		{
			UGameplayStatics::ApplyDamage(DamageMyFirstPersonCharacter, Damage, nullptr, GetInstigator()->GetInstigator(), nullptr);
			Destroy();
		}
	}
}

void ARifleAmmoActor::ExplodeBySelf()
{
	GetWorldTimerManager().ClearTimer(AmmoTimerHandle);
	Destroy();
}

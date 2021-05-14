// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeAmmoActor.h"


#include "AI/AICharacter.h"
#include "ConstructorHelpers.h"
#include "Widget/DamageWidget.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "ParticleDefinitions.h"
#include "TextBlock.h"
#include "WidgetComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Sound/SoundBase.h"

AGrenadeAmmoActor::AGrenadeAmmoActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Use a sphere as a simple collision representation
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &AGrenadeAmmoActor::OnHit);   // set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComponent->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComponent;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	
	// Set static mesh
	AmmoMeshComponent->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> AmmoMesh(TEXT("StaticMesh'/Game/FPWeapon/Ammo/Gun0/FirstPersonProjectileMesh.FirstPersonProjectileMesh'"));
	if (AmmoMesh.Succeeded())
	{
		AmmoMeshComponent->SetStaticMesh(AmmoMesh.Object);
        AmmoMeshComponent->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));
	}
	
	// Die after 3 seconds by default
	// InitialLifeSpan = 3.0f;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionParticle(TEXT("ParticleSystem'/Game/Effects/SciFiWeapLight/FX/Particles/P_Impact_Wood_Small_Dark.P_Impact_Wood_Small_Dark'"));
	if (ExplosionParticle.Succeeded())
	{
		AmmoExplosionParticle = ExplosionParticle.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> ExplosionSound(TEXT("SoundWave'/Game/Effects/SciFiWeapLight/Sound/SniperRifle/Wavs/SR_ImpactBody03.SR_ImpactBody03'"));
	if (ExplosionSound.Succeeded())
	{
		AmmoExplosionSound = ExplosionSound.Object;
	}

	Damage = 20.0f;

	DamageTime = 2.5f;

}

void AGrenadeAmmoActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(AmmoTimerHandle, this, &AAmmoBaseActor::ExplodeBySelf, 3.0f);
}

void AGrenadeAmmoActor::Tick(float DeltaTime)
{
}

void AGrenadeAmmoActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		AAICharacter* DamageCharacter = Cast<AAICharacter>(OtherActor);
		if (DamageCharacter && DamageCharacter->CurrentHealth > 0.1f)
		{
			if (Hit.Location.Z > DamageCharacter->HeadLowBoundZ)
			{
				UGameplayStatics::ApplyDamage(DamageCharacter, DamageTime * Damage, nullptr, GetInstigator(), nullptr);
				SpawnFloatingDamage(DamageTime * Damage);
			}
			else
			{
				UGameplayStatics::ApplyDamage(DamageCharacter, Damage, nullptr, GetInstigator(), nullptr);
				SpawnFloatingDamage(Damage);
			}
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AmmoExplosionParticle, GetActorLocation(), GetActorRotation());
			UGameplayStatics::PlaySoundAtLocation(this, AmmoExplosionSound, GetActorLocation());
			Destroy();
		}
	}
	
	// Only add impulse and destroy projectile if we hit a physics
	if (OtherActor && (OtherActor!=this) && OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
		// Destroy();
	}
}

void AGrenadeAmmoActor::ExplodeBySelf()
{
	GetWorldTimerManager().ClearTimer(AmmoTimerHandle);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AmmoExplosionParticle, GetActorLocation(), GetActorRotation());
	UGameplayStatics::PlaySoundAtLocation(this, AmmoExplosionSound, GetActorLocation());
	Destroy();
}


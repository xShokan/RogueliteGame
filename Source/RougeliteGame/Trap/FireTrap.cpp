// Fill out your copyright notice in the Description page of Project Settings.


#include "FireTrap.h"

#include "ConstructorHelpers.h"
#include "MultiBoxDefs.h"
#include "FirstPersonCharacter.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Particles/ParticleSystem.h"
#include "ParticleDefinitions.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AFireTrap::AFireTrap()
{
	PrimaryActorTick.bCanEverTick = false;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>StaticMesh(TEXT("StaticMesh'/Game/LevelThings/SM_FireTrap.SM_FireTrap'"));
	if (StaticMesh.Succeeded())
	{
		TrapMeshComponent->SetStaticMesh(StaticMesh.Object);
		FVector BoxExtent = TrapMeshComponent->GetStaticMesh()->GetBounds().BoxExtent;
		CollisionComponent->InitBoxExtent(BoxExtent);
	}

	CollisionComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	CollisionComponent->SetRelativeScale3D(FVector(1.0f, 1.0f, 30.0f));
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Trap"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem>Particle(TEXT("ParticleSystem'/Game/Effects/SciFiWeapLight/FX/Particles/P_SniperRifle_Tracer_Dark.P_SniperRifle_Tracer_Dark'"));
	if (Particle.Succeeded())
	{
		FireParticle = Particle.Object;
	}

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(RootComponent);
	ParticleSystemComponent->SetTemplate(FireParticle);
	ParticleSystemComponent->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	ParticleSystemComponent->SetRelativeScale3D(FVector(4.5f, 4.5f, 3.5f));
	ParticleSystemComponent->OnSystemFinished.AddDynamic(this, &AFireTrap::ParticleFinished);
	
	
	bParticleFinished = false;
	Damage = 1.0f;
	StartTime = 0.1f;
	CycleTime = 6.0f;
	DamageTimes = 5;
	// UE_LOG(LogTemp, Warning, TEXT("AFireTrap::AFireTrap()"));
}

void AFireTrap::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle handle;

	GetWorld()->GetTimerManager().SetTimer(handle, this, &AFireTrap::PlayFire, StartTime, false);
}

void AFireTrap::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
}

void AFireTrap::ParticleFinished(UParticleSystemComponent* PSystem)
{
	// UE_LOG(LogTemp, Warning, TEXT("Particle Finished"));
	bParticleFinished = true;
}

void AFireTrap::OnOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult)
{
	Super::OnOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	
	/*GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, [this]() {
		if (DamageCharacter && !bParticleFinished && bOverlapping)
		{
            UGameplayStatics::ApplyDamage(DamageCharacter, Damage, nullptr, nullptr, nullptr);
        }
    }, 0.5f, true);*/
	if (DamageCharacter && !bParticleFinished && !DamageCharacter->bBurning)
	{
		DamageCharacter->bBurning = true;
		DamageCharacter->ParticleComponent->Activate();
		GetWorld()->GetTimerManager().SetTimer(BurningTimerHandle, this, &AFireTrap::BurningDamage, 0.5f, true);
	}
	
}

void AFireTrap::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	Super::EndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AFireTrap::PlayFire()
{
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
        // UE_LOG(LogTemp, Warning, TEXT("######################"));
        ParticleSystemComponent->Activate(true);
        bParticleFinished = false;
        if (DamageCharacter && !bParticleFinished && !DamageCharacter->bBurning && bOverlapping)
        {
        	DamageCharacter->bBurning = true;
			DamageCharacter->ParticleComponent->Activate();
        	GetWorld()->GetTimerManager().SetTimer(BurningTimerHandle, this, &AFireTrap::BurningDamage, 0.5f, true);
        }
    }, CycleTime, true);
}

void AFireTrap::BurningDamage()
{
	if (DamageTimes > 0 && DamageCharacter)
	{
		UGameplayStatics::ApplyDamage(DamageCharacter, Damage, nullptr, nullptr, nullptr);
		if (!bOverlapping && !bParticleFinished)
		{
			DamageTimes--;
		}
	}
	else
	{
		DamageCharacter->bBurning = false;
		DamageCharacter->ParticleComponent->Deactivate();
		GetWorld()->GetTimerManager().ClearTimer(BurningTimerHandle);
		DamageTimes = 5;
	}
}

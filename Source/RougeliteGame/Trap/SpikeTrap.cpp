// Fill out your copyright notice in the Description page of Project Settings.


#include "SpikeTrap.h"


#include "ConstructorHelpers.h"
#include "FirstPersonCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"

ASpikeTrap::ASpikeTrap()
{
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/LevelThings/SM_SpikeTrap.SM_SpikeTrap'"));
	if (StaticMesh.Succeeded())
	{
		TrapMeshComponent->SetStaticMesh(StaticMesh.Object);
		FVector BoxExtent = TrapMeshComponent->GetStaticMesh()->GetBounds().BoxExtent;
		CollisionComponent->InitBoxExtent(BoxExtent);
	}

	TrapMeshComponent->BodyInstance.SetCollisionProfileName(TEXT("Trap"));
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Trap"));
	CollisionComponent->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.5f));
	
	Damage = 10.0f;
	// UE_LOG(LogTemp, Warning, TEXT("ASpikeTrap::ASpikeTrap()"));
	// UE_LOG(LogTemp, Warning, TEXT("%f"), Damage);
}

void ASpikeTrap::BeginPlay()
{
}

void ASpikeTrap::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (DamageCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("ASpikeTrap::OnOverlap"));
		UGameplayStatics::ApplyDamage(DamageCharacter, Damage, nullptr, nullptr, nullptr);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DamageCharacter Fail"));
	}
}

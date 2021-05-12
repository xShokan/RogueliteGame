// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoReward.h"


#include "ConstructorHelpers.h"
#include "FirstPersonCharacter.h"
#include "Weapon/WeaponBaseActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

AAmmoReward::AAmmoReward()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/LevelThings/SM_AmmoReward.SM_AmmoReward'"));
	if (StaticMesh.Succeeded())
	{
		RewardMeshComponent->SetStaticMesh(StaticMesh.Object);
		CollisionComponent->InitBoxExtent(RewardMeshComponent->GetStaticMesh()->GetBounds().BoxExtent);
	}
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AAmmoReward::OnOverlap);
}

void AAmmoReward::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (RewardCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("AAmmoReward::OnOverlap"));
		RewardCharacter->Weapon->FillAmmo();
		Destroy();
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "RewardBase.h"


#include "FirstPersonCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

// Sets default values
ARewardBase::ARewardBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RewardMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RewardMeshComponent"));
	RootComponent = RewardMeshComponent;
	
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->BodyInstance.SetCollisionProfileName("Trap");
}

// Called when the game starts or when spawned
void ARewardBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARewardBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARewardBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	RewardCharacter = Cast<AFirstPersonCharacter>(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("ARewardBase::OnOverlap"));
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapBase.h"


#include "FirstPersonCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATrapBase::ATrapBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponet"));
	
	TrapMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrapMeshComponent"));
	RootComponent = TrapMeshComponent;
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATrapBase::OnOverlap);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ATrapBase::EndOverlap);

	bOverlapping = false;
    // UE_LOG(LogTemp, Warning, TEXT("ATrapBase::ATrapBase()"));
}

// Called when the game starts or when spawned
void ATrapBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrapBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrapBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bOverlapping = true;
	DamageCharacter = Cast<AFirstPersonCharacter>(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("TrapBase OnOverlap"));
}

void ATrapBase::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	bOverlapping = false;
}


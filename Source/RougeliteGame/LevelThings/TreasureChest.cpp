// Fill out your copyright notice in the Description page of Project Settings.


#include "TreasureChest.h"



#include "AmmoReward.h"
#include "ConstructorHelpers.h"
#include "FirstPersonCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

// Sets default values
ATreasureChest::ATreasureChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
    static ConstructorHelpers::FObjectFinder<UStaticMesh>StaticMesh(TEXT("StaticMesh'/Game/LevelThings/SM_TreasureChest.SM_TreasureChest'"));
    if (StaticMesh.Succeeded())
    {
	    StaticMeshComponent->SetStaticMesh(StaticMesh.Object);
    }
	// StaticMeshComponent->BodyInstance.SetCollisionProfileName("LevelThings");
	RootComponent = StaticMeshComponent;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>("CollisionComponent");
	CollisionComponent->SetupAttachment(RootComponent);
	// CollisionComponent->BodyInstance.SetCollisionProfileName("Trap");
	FVector BoxExtent = StaticMeshComponent->GetStaticMesh()->GetBounds().BoxExtent;
	BoxExtent = BoxExtent + 10;
	CollisionComponent->InitBoxExtent(BoxExtent);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATreasureChest::OnOverlap);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ATreasureChest::EndOverlap);

	bCanSpawnTreasure = true;
}

// Called when the game starts or when spawned
void ATreasureChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATreasureChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATreasureChest::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OverlappingCharacter = Cast<AFirstPersonCharacter>(OtherActor);
	if (OverlappingCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("ATreasureChest::OnOverlap"));
		OverlappingCharacter->bOverlappingTreasureChest = true;
		OverlappingCharacter->TreasureChest = this;
	}
}

void ATreasureChest::EndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OverlappingCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("ATreasureChest::EndOverlap"));
		OverlappingCharacter->bOverlappingTreasureChest = false;\
		OverlappingCharacter->TreasureChest = nullptr;
	}
}

void ATreasureChest::Open()
{
	if (bCanSpawnTreasure)
	{
		UE_LOG(LogTemp, Warning, TEXT("ATreasureChest::Open"));

		FActorSpawnParameters SpawnParameters;
		FTransform SpawnTransform;
		FVector SpawnLocation = GetActorLocation();
		SpawnParameters.bNoFail = true;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		SpawnLocation.Z += 80;
		SpawnTransform.SetLocation(SpawnLocation);
	
		GetWorld()->SpawnActor<AAmmoReward>(AAmmoReward::StaticClass(), SpawnTransform, SpawnParameters);
		bCanSpawnTreasure = false;
	}
}


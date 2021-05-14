// Fill out your copyright notice in the Description page of Project Settings.


#include "TreasureChest.h"


#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"

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
	RootComponent = StaticMeshComponent;

	

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


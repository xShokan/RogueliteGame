// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingDamage.h"

#include "ConstructorHelpers.h"
#include "UserWidget.h"
#include "WidgetComponent.h"

// Sets default values
AFloatingDamage::AFloatingDamage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DamageWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DamageFigure"));
	RootComponent = DamageWidgetComponent;
	static ConstructorHelpers::FClassFinder<UUserWidget> DamageWidgetClass(TEXT("WidgetBlueprint'/Game/Blueprints/WBP_Damage.WBP_Damage_C'"));
	if (DamageWidgetClass.Succeeded())
	{
		DamageWidgetComponent->SetWidgetClass(DamageWidgetClass.Class);
	}
	DamageWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	DamageWidgetComponent->SetDrawAtDesiredSize(true);
}

// Called when the game starts or when spawned
void AFloatingDamage::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFloatingDamage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


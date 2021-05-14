// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBaseActor.h"


#include "AI/AICharacter.h"
#include "ConstructorHelpers.h"
#include "Widget/DamageWidget.h"
#include "Widget/FloatingDamage.h"
#include "GrenadeAmmoActor.h"
#include "TextBlock.h"
#include "TimerManager.h"
#include "WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAmmoBaseActor::AAmmoBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	AmmoMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ammo"));
}

// Called when the game starts or when spawned
void AAmmoBaseActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAmmoBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AAmmoBaseActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{

}

void AAmmoBaseActor::SpawnFloatingDamage(float ThisDamage)
{
	AFloatingDamage* FloatingDamage = GetWorld()->SpawnActor<AFloatingDamage>(GetActorLocation(), GetActorRotation());
	UWidgetComponent* WidgetComponent = Cast<UWidgetComponent>(FloatingDamage->GetComponentByClass(UWidgetComponent::StaticClass()));
	if (WidgetComponent)
	{
		UDamageWidget* DamageWidget = Cast<UDamageWidget>(WidgetComponent->GetUserWidgetObject());
		if (DamageWidget)
		{
			DamageWidget->DamageFigureLabel->SetText(FText::AsNumber(ThisDamage));
			if (ThisDamage > Damage)
			{
				DamageWidget->DamageFigureLabel->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 1.0f, 0.0f)));
			}
		}
	}
	FloatingDamage->SetLifeSpan(1.0f);
}


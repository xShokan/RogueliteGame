// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"



#include "AIController.h"
#include "Widget/AIHealthWidget.h"
#include "ConstructorHelpers.h"
#include "Weapon/RifleGunActor.h"
#include "HumanAIController.h"
#include "WidgetComponent.h"
#include "Animation/AnimBlueprint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIMeshComponent = GetMesh();
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
    if (SkeletalMesh.Succeeded())
    {
	    AIMeshComponent->SetSkeletalMesh(SkeletalMesh.Object);
        AIMeshComponent->RelativeLocation = FVector(0.0f, 0.0f, -90.0f);
        AIMeshComponent->RelativeRotation = FRotator(0, -90, 0);
    }
	
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBlueprint(TEXT("AnimBlueprint'/Game/Animation/BP_AICharacterAnim.BP_AICharacterAnim'"));
    if (AnimBlueprint.Succeeded())
    {
	    AIMeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
        AIMeshComponent->SetAnimClass(AnimBlueprint.Object->GeneratedClass);
    }

	static ConstructorHelpers::FObjectFinder<UAnimationAsset> DeathAnimationAsset(TEXT("AnimSequence'/Game/AnimStarterPack/Death_3.Death_3'"));
    if (DeathAnimationAsset.Succeeded())
    {
	    DeathAnimation = DeathAnimationAsset.Object;
    }
	
	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBarComponent->SetupAttachment(RootComponent);
	static ConstructorHelpers::FClassFinder<UUserWidget> Widget(TEXT("WidgetBlueprint'/Game/Blueprints/WBP_AIHealth.WBP_AIHealth_C'"));
    if (Widget.Succeeded())
    {
    	HealthBarComponent->SetWidgetClass(Widget.Class);
    	HealthBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
    	HealthBarComponent->SetDrawAtDesiredSize(true);
    }

	AIControllerClass = AHumanAIController::StaticClass();
	
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	
	GetCharacterMovement()->MaxWalkSpeed = 120.0f;

	bUseControllerRotationYaw = true;
}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	if (AIControllerClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIControllerClass:%s"), *AIControllerClass->GetName());
	}
	UAIHealthWidget *HealthBar = Cast<UAIHealthWidget>(HealthBarComponent->GetUserWidgetObject());
	HealthBar->SetOwnerCharacter(this);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.bNoFail = true;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FTransform WeaponTransform;
	WeaponTransform.SetLocation(FVector::ZeroVector);
	WeaponTransform.SetRotation(FQuat(FRotator::ZeroRotator));
	Weapon = GetWorld()->SpawnActor<ARifleGunActor>(ARifleGunActor::StaticClass(), WeaponTransform, SpawnParameters);
	if (Weapon)
	{
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GripPoint"));
		Weapon->Instigator = this;
	}

	HealthBarComponent->SetVisibility(false);
}

// Called every frame
void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HealthBarComponent->SetWorldLocation(AIMeshComponent->GetSocketLocation(TEXT("HealthBarSocket")));
}

// Called to bind functionality to input
void AAICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);
	OnTakeDamage.Broadcast(CurrentHealth, MaxHealth);
	HealthBarComponent->SetVisibility(true);
	// GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Orange, FString::SanitizeFloat(CurrentHealth));
	if (CurrentHealth < 0.1f)
	{
		AHumanAIController* MyAIController = Cast<AHumanAIController>(GetController());
		MyAIController->GetBlackboardComponent()->SetValueAsBool(FName("bDied"), true);
		AIMeshComponent->PlayAnimation(DeathAnimation, false);
		Weapon->SetLifeSpan(5.0f);
		SetLifeSpan(5.0f);
	}
	return 0.0f;
}

void AAICharacter::UpdateWalkSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void AAICharacter::Fire()
{
	Weapon->Fire();
}


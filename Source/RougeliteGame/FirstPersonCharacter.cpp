// Fill out your copyright notice in the Description page of Project Settings.

#include "FirstPersonCharacter.h"

#include "ConstructorHelpers.h"
#include "GeometryCollectionSimulationCoreTypes.h"
#include "RougeliteGameGameModeBase.h"
#include "Weapon/GrenadeGunActor.h"
#include "Weapon/RifleGunActor.h"
#include "Animation/AnimBlueprint.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFirstPersonCharacter::AFirstPersonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(RootComponent);
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.0f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	
	// Set a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = GetMesh();
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.190001f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.699997f));

	// Set character skeletal mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh1P(TEXT("/Game/FirstPerson/Character/Mesh/SK_Mannequin_Arms"));
    if (SkeletalMesh1P.Succeeded())
    {
	    Mesh1P->SetSkeletalMesh(SkeletalMesh1P.Object);
    }
	
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBlueprint(TEXT("AnimBlueprint'/Game/Animation/BP_MyCharacterAnim.BP_MyCharacterAnim'"));
    if (AnimBlueprint.Succeeded())
    {
	    Mesh1P->SetAnimationMode(EAnimationMode::AnimationBlueprint);
        Mesh1P->SetAnimInstanceClass(AnimBlueprint.Object->GeneratedClass);
    }

	// Set fire montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FireMontage(TEXT("AnimMontage'/Game/FirstPerson/Animations/FirstPersonFire_Montage.FirstPersonFire_Montage'"));
    if (FireMontage.Succeeded())
    {
	    FireAnimation = FireMontage.Object;
    }

	// Set reload montage
	/*static ConstructorHelpers::FObjectFinder<UAnimMontage> ReloadMontage(TEXT("AnimMontage'/Game/Animation/FPSReload_Rifle_Hip_Montage.FPSReload_Rifle_Hip_Montage'"));
	if (ReloadMontage.Succeeded())
	{
		ReloadAnimation = ReloadMontage.Object;
	}*/

	
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth - 10.0f;
	
	WeaponArray.Init(Weapon, 2);
	// 0 is default weapon
	WeaponIndex = 0;

	Tags.Add(FName("Player"));

	bAim = false;
}

// Called when the game starts or when spawned
void AFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.bNoFail = true;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FTransform WeaponTransform;
	WeaponTransform.SetLocation(FVector::ZeroVector);
	WeaponTransform.SetRotation(FQuat(FRotator::ZeroRotator));
	
	WeaponArray[0] = GetWorld()->SpawnActor<ARifleGunActor>(ARifleGunActor::StaticClass(), WeaponTransform, SpawnParameters);
	if (WeaponArray[0])
	{
		WeaponArray[0]->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GripPoint"));
		WeaponArray[0]->Instigator = this;
		// WeaponArray[0]->SetActorHiddenInGame(true);
		Weapon = WeaponArray[WeaponIndex];
	}

	WeaponArray[1] = GetWorld()->SpawnActor<AGrenadeGunActor>(AGrenadeGunActor::StaticClass(), WeaponTransform, SpawnParameters);
	if (WeaponArray[1])
	{
		WeaponArray[1]->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GripPoint"));
		WeaponArray[1]->Instigator = this;
		WeaponArray[1]->SetActorHiddenInGame(true);
	}

	RougeliteGameGameMode = Cast<ARougeliteGameGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (RougeliteGameGameMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode Succeed"));
		RougeliteGameGameMode->OnTakeDamage.Broadcast(CurrentHealth, MaxHealth, Weapon->AmmoNumInClip, Weapon->AmmoTotalNum, Weapon->Name);
	}

}

void AFirstPersonCharacter::MoveForward(float Val)
{
	if (Val != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Val);
	}
}

void AFirstPersonCharacter::MoveRight(float Val)
{
	if (Val != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Val);
	}
}

void AFirstPersonCharacter::Fire()
{
	/*if (FireAnimation)
	{
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(FireAnimation);
		}
	}*/
	Weapon->Fire();
	RougeliteGameGameMode->OnTakeDamage.Broadcast(CurrentHealth, MaxHealth, Weapon->AmmoNumInClip, Weapon->AmmoTotalNum, Weapon->Name);
}

void AFirstPersonCharacter::Reload()
{
	/*if (ReloadAnimation)
	{
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("ReloadAnimation"));
			AnimInstance->Montage_Play(ReloadAnimation);
		}
	}*/
	Weapon->Reload();
	RougeliteGameGameMode->OnTakeDamage.Broadcast(CurrentHealth, MaxHealth, Weapon->AmmoNumInClip, Weapon->AmmoTotalNum, Weapon->Name);
}

void AFirstPersonCharacter::SwitchWeapon()
{
	Weapon->SetActorHiddenInGame(true);
	if (WeaponIndex < WeaponArray.Num() - 1)
	{
		WeaponIndex = WeaponIndex + 1;
	}
	else
	{
		WeaponIndex = 0;
	}
	Weapon = WeaponArray[WeaponIndex];
	Weapon->SetActorHiddenInGame(false);
}

void AFirstPersonCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 800.f;
}

void AFirstPersonCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

void AFirstPersonCharacter::StartAim()
{
	if (Cast<ARifleGunActor>(Weapon))
	{
		bAim = true;
		FirstPersonCameraComponent->SetRelativeLocation(FVector(35.439999f, 1.75f, 54.0f)); // Position the camera
		Mesh1P->SetRelativeLocation(FVector(-60.0f, -13.0f, -143.0f));
	}
}

void AFirstPersonCharacter::StopAim()
{
	if (Cast<ARifleGunActor>(Weapon))
	{
		bAim = false;
		FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.0f)); // Position the camera
		Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.699997f));
	}
}

// Called every frame
void AFirstPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AFirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AFirstPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AFirstPersonCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AFirstPersonCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AFirstPersonCharacter::AddControllerPitchInput);
	
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AFirstPersonCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AFirstPersonCharacter::StopJumping);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AFirstPersonCharacter::Fire);
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &AFirstPersonCharacter::Reload);
	PlayerInputComponent->BindAction(TEXT("SwitchWeapon"), IE_Pressed, this, &AFirstPersonCharacter::SwitchWeapon);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AFirstPersonCharacter::StartSprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AFirstPersonCharacter::StopSprint);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Pressed, this, &AFirstPersonCharacter::StartAim);
	PlayerInputComponent->BindAction(TEXT("Aim"), IE_Released, this, &AFirstPersonCharacter::StopAim);

}

float AFirstPersonCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);
	RougeliteGameGameMode->OnTakeDamage.Broadcast(CurrentHealth, MaxHealth, Weapon->AmmoNumInClip, Weapon->AmmoTotalNum, Weapon->Name);
	return 0.0f;
}


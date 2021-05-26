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
#include "LevelThings/TreasureChest.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AFirstPersonCharacter::AFirstPersonCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(RootComponent);
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-29.56f, 1.75f, 54.0f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	
	// Set a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	FirstPersonMesh = GetMesh();
	FirstPersonMesh->SetupAttachment(FirstPersonCameraComponent);
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;
	FirstPersonMesh->SetRelativeRotation(FRotator(1.9f, -19.190001f, 5.2f));
	FirstPersonMesh->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.699997f));
	FirstPersonMesh->SetOnlyOwnerSee(true);

	// Set a mesh component that will be used when being viewed from other players
	ThirdPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ThirdPersonMesh"));
	ThirdPersonMesh->SetupAttachment(RootComponent);
	ThirdPersonMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	ThirdPersonMesh->SetRelativeLocation(FVector(-0.0f, -0.0f, -90.0f));
	ThirdPersonMesh->SetOwnerNoSee(true);

	// Set character skeletal mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FirstPersonSkeletalMesh(TEXT("/Game/FirstPerson/Character/Mesh/SK_Mannequin_Arms"));
    if (FirstPersonSkeletalMesh.Succeeded())
    {
	    FirstPersonMesh->SetSkeletalMesh(FirstPersonSkeletalMesh.Object);
    }

	// Set character skeletal mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> ThirdPersonSkeletalMesh(TEXT("SkeletalMesh'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (ThirdPersonSkeletalMesh.Succeeded())
	{
		ThirdPersonMesh->SetSkeletalMesh(ThirdPersonSkeletalMesh.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> FirstPersonAnim(TEXT("AnimBlueprint'/Game/Animation/BP_MyCharacterAnim.BP_MyCharacterAnim'"));
    if (FirstPersonAnim.Succeeded())
    {
	    FirstPersonMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
        FirstPersonMesh->SetAnimInstanceClass(FirstPersonAnim.Object->GeneratedClass);
    }

	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> ThirdPersonAnim(TEXT("AnimBlueprint'/Game/Animation/BP_ThirdPersopnAnim.BP_ThirdPersopnAnim'"));
	if (ThirdPersonAnim.Succeeded())
	{
		ThirdPersonMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		ThirdPersonMesh->SetAnimInstanceClass(ThirdPersonAnim.Object->GeneratedClass);
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

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));
	ParticleComponent->SetAutoActivate(false);
	ParticleComponent->SetupAttachment(GetMesh());
	static ConstructorHelpers::FObjectFinder<UParticleSystem>Burning(TEXT("ParticleSystem'/Game/M5VFXVOL2/Particles/Fire/Fire_12.Fire_12'"));
	if (Burning.Succeeded())
	{
		ParticleComponent->SetTemplate(Burning.Object);
	}
	// ParticleComponent->ActivateSystem(false);

	
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth - 10.0f;
	
	WeaponArray.Init(Weapon, 2);
	WeaponArrayCopy.Init(WeaponCopy, 2);
	// 0 is default weapon
	WeaponIndex = 0;

	Tags.Add(FName("Player"));

	bAim = false;
	bBurning = false;
	bOverlappingTreasureChest = false;
	GoldNum = 0;
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

	// Real weapon
	if (GetLocalRole() == ROLE_Authority)
	{
		WeaponArray[0] = GetWorld()->SpawnActor<ARifleGunActor>(ARifleGunActor::StaticClass(), WeaponTransform, SpawnParameters);
		if (WeaponArray[0])
		{
			WeaponArray[0]->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GripPoint"));
			WeaponArray[0]->Instigator = this;
			WeaponArray[0]->SetOwner(this);
			// WeaponArray[0]->SetActorHiddenInGame(true);
			Weapon = WeaponArray[WeaponIndex];
		}

		WeaponArray[1] = GetWorld()->SpawnActor<AGrenadeGunActor>(AGrenadeGunActor::StaticClass(), WeaponTransform, SpawnParameters);
		if (WeaponArray[1])
		{
			WeaponArray[1]->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GripPoint"));
			WeaponArray[1]->Instigator = this;
			WeaponArray[1]->SetOwner(this);
			WeaponArray[1]->SetActorHiddenInGame(true);
		}
	}

	// Copy weapon
	if (IsLocallyControlled())
	{
		WeaponArrayCopy[0] = GetWorld()->SpawnActor<ARifleGunActor>(ARifleGunActor::StaticClass(), WeaponTransform, SpawnParameters);
		if (WeaponArrayCopy[0])
		{
			WeaponArrayCopy[0]->AttachToComponent(ThirdPersonMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GripPoint"));
			WeaponCopy = WeaponArrayCopy[WeaponIndex];
		}

		WeaponArrayCopy[1] = GetWorld()->SpawnActor<AGrenadeGunActor>(AGrenadeGunActor::StaticClass(), WeaponTransform, SpawnParameters);
		if (WeaponArrayCopy[1])
		{
			WeaponArrayCopy[1]->AttachToComponent(ThirdPersonMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GripPoint"));
			WeaponArrayCopy[1]->SetActorHiddenInGame(true);
		}
	}


	if (OnUIChange.IsBound() && OnGoldAdd.IsBound() && Weapon)
	{
		OnUIChange.Broadcast(CurrentHealth, MaxHealth, Weapon->AmmoNumInClip, Weapon->AmmoTotalNum, Weapon->Name);
		OnGoldAdd.Broadcast(GoldNum);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AFirstPersonCharacter::BeginPlay not IsBound"));
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
	HandleFireOnServer();
}

void AFirstPersonCharacter::HandleFireOnServer_Implementation()
{
	if (Weapon->AmmoNumInClip > 0)
	{
		PlayMontageMulticast();
		Weapon->Fire();
        OnUIChange.Broadcast(CurrentHealth, MaxHealth, Weapon->AmmoNumInClip, Weapon->AmmoTotalNum, Weapon->Name);
	}
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
	OnUIChange.Broadcast(CurrentHealth, MaxHealth, Weapon->AmmoNumInClip, Weapon->AmmoTotalNum, Weapon->Name);
}

void AFirstPersonCharacter::SwitchWeapon()
{
	HandleSwitchWeaponOnSever();
}

void AFirstPersonCharacter::HandleSwitchWeaponOnSever_Implementation()
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
		FirstPersonMesh->SetRelativeLocation(FVector(-60.0f, -13.0f, -143.0f));
	}
}

void AFirstPersonCharacter::StopAim()
{
	if (Cast<ARifleGunActor>(Weapon))
	{
		bAim = false;
		FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.0f)); // Position the camera
		FirstPersonMesh->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.699997f));
	}
}

void AFirstPersonCharacter::Open()
{
	if (bOverlappingTreasureChest && TreasureChest)
	{
		TreasureChest->Open();
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
	PlayerInputComponent->BindAction(TEXT("Interaction"), IE_Pressed, this, &AFirstPersonCharacter::Open);

}

float AFirstPersonCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);
	if (OnUIChange.IsBound())
	{
		OnUIChange.Broadcast(CurrentHealth, MaxHealth, Weapon->AmmoNumInClip, Weapon->AmmoTotalNum, Weapon->Name);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AFirstPersonCharacter::TakeDamage not IsBound"));
	}
	return 0.0f;
}

void AFirstPersonCharacter::PlayMontageMulticast_Implementation()
{
	if (FireAnimation)
	{
		PlayAnimMontage(FireAnimation);
	}
}

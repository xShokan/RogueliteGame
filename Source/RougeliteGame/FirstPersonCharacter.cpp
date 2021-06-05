// Fill out your copyright notice in the Description page of Project Settings.

#include "FirstPersonCharacter.h"
#include "CharacterPlayerState.h"
#include "ConstructorHelpers.h"
#include "GeometryCollectionSimulationCoreTypes.h"
#include "TimerManager.h"
#include "UnrealNetwork.h"
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
	PrimaryActorTick.bCanEverTick = true;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(RootComponent);
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.439999, 1.75f, 74.0f)); // Position the camera
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

	// Set first person fire montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FirstPersonFireMontage(TEXT("AnimMontage'/Game/FirstPerson/Animations/FirstPersonFire_Montage.FirstPersonFire_Montage'"));
    if (FirstPersonFireMontage.Succeeded())
    {
	    FirstPersonFireAnimation = FirstPersonFireMontage.Object;
    }

	// Set third person fire montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ThirdPersonFireMontage(TEXT("AnimMontage'/Game/Animation/Fire_Rifle_Ironsights_Montage.Fire_Rifle_Ironsights_Montage'"));
	if (ThirdPersonFireMontage.Succeeded())
	{
		ThirdPersonFireAnimation = ThirdPersonFireMontage.Object;
	}

	// Set first person reload montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FirstPersonReloadMontage(TEXT("AnimMontage'/Game/Animation/Reload_Rifle_Ironsights_PreviewMesh_Montage.Reload_Rifle_Ironsights_PreviewMesh_Montage'"));
	if (FirstPersonReloadMontage.Succeeded())
	{
		FirstPersonReloadAnimation = FirstPersonReloadMontage.Object;
	}

	// Set third person reload montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ThirdPersonReloadMontage(TEXT("AnimMontage'/Game/Animation/Reload_Rifle_Ironsights_Montage.Reload_Rifle_Ironsights_Montage'"));
	if (ThirdPersonReloadMontage.Succeeded())
	{
		ThirdPersonReloadAnimation = ThirdPersonReloadMontage.Object;
	}

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));
	ParticleComponent->SetAutoActivate(false);
	ParticleComponent->SetupAttachment(GetMesh());
	static ConstructorHelpers::FObjectFinder<UParticleSystem>Burning(TEXT("ParticleSystem'/Game/M5VFXVOL2/Particles/Fire/Fire_12.Fire_12'"));
	if (Burning.Succeeded())
	{
		ParticleComponent->SetTemplate(Burning.Object);
	}
	// ParticleComponent->ActivateSystem(false);

	/*FirstPersonMesh->HideBoneByName(FName("upperarm_l"), EPhysBodyOp::PBO_None);
	FirstPersonMesh->HideBoneByName(FName("lowerarm_l"), EPhysBodyOp::PBO_None);
	FirstPersonMesh->HideBoneByName(FName("hand_l"), EPhysBodyOp::PBO_None);*/
	
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth - 10.0f;
	
	WeaponArray.Init(Weapon, 2);
	/*WeaponArrayFirstPerson.Init(WeaponFirstPerson, 2);
	WeaponArrayThirdPerson.Init(WeaponThirdPerson, 2);*/
	// 0 is default weapon
	WeaponIndex = 0;

	Tags.Add(FName("Player"));

	bAim = false;
	bBurning = false;
	bOverlappingTreasureChest = false;
	GoldNum = 0;
	// bUseControllerRotationPitch = true;
	// bReplicates = true;

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
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
	if (true)
	{
		WeaponArray[0] = GetWorld()->SpawnActor<ARifleGunActor>(ARifleGunActor::StaticClass(), WeaponTransform, SpawnParameters);
		if (WeaponArray[0])
		{
			// WeaponArray[0]->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GripPoint"));
			BindWeaponOnMesh(WeaponArray[0]);
			WeaponArray[0]->Instigator = this;
			Weapon = WeaponArray[WeaponIndex];
		}

		WeaponArray[1] = GetWorld()->SpawnActor<AGrenadeGunActor>(AGrenadeGunActor::StaticClass(), WeaponTransform, SpawnParameters);
		if (WeaponArray[1])
		{
			// WeaponArray[1]->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GripPoint"));
			// BindWeaponOnMesh(WeaponArray[1]);
			WeaponArray[1]->Instigator = this;
			WeaponArray[1]->SetActorHiddenInGame(true);
		}
		// Weapon->WeaponMeshComponent->SetOnlyOwnerSee(true);
		/*FTimerHandle Handle;
		GetWorld()->GetTimerManager().SetTimer(Handle, [this]() {
			OnGameInfoUIUpdate(CurrentHealth, MaxHealth, Weapon->AmmoNumInClip, Weapon->AmmoTotalNum, Weapon->Name);
		}, 2.0f, false);*/
	}
	if (IsLocallyControlled())
	{
		BeginGameInfoUpdateOnServer();
	}

	TArray<class AActor*> FirstPersonCharacterArray;
	UGameplayStatics::GetAllActorsOfClass(this, AFirstPersonCharacter::StaticClass(), FirstPersonCharacterArray);

	for (int i=0;i<FirstPersonCharacterArray.Num();i++)
	{
		if (FirstPersonCharacterArray[i] != this)
		{
			OtherCharacter = Cast<AFirstPersonCharacter>(FirstPersonCharacterArray[i]);
			if (ACharacterPlayerState* PS = OtherCharacter->GetPlayerState<ACharacterPlayerState>())
			{
				PS->OtherPlayerName = OtherCharacter->GetName();
				PS->OtherPlayerMaxHealth = OtherCharacter->MaxHealth;
				// PS->ChangeOtherPlayerCurrentHealthOnSever(CurrentHealth);

				FTimerHandle Handle;
				GetWorld()->GetTimerManager().SetTimer(Handle, [this]
				{
					ACharacterPlayerState* PS = OtherCharacter->GetPlayerState<ACharacterPlayerState>();
					PS->ChangeOtherPlayerCurrentHealthOnSever(CurrentHealth);
				}, 3.0f, false);
			}
		}
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
		PlayFireMontageSoundMulticast();
		Weapon->Fire();
        OnGameInfoUIUpdate(CurrentHealth, MaxHealth, Weapon->AmmoNumInClip, Weapon->AmmoTotalNum, Weapon->Name);
		// CharacterInfo.CurrentHealth--;
		// CurrentHealth--;
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
	/*Weapon->Reload();
	OnUIChange.Broadcast(CurrentHealth, MaxHealth, Weapon->AmmoNumInClip, Weapon->AmmoTotalNum, Weapon->Name);*/
	if (IsLocallyControlled())
	{
		HandleReloadOnSever();
	}
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
	OnGameInfoUIUpdate(CurrentHealth, MaxHealth, Weapon->AmmoNumInClip, Weapon->AmmoTotalNum, Weapon->Name);
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

void AFirstPersonCharacter::StartCrouch()
{
	Crouch();
}

void AFirstPersonCharacter::EndCrouch()
{
	UnCrouch();
}

void AFirstPersonCharacter::OpenTreasureChest()
{
	if (bOverlappingTreasureChest && TreasureChest)
	{
		TreasureChest->Open();
	}
}

void AFirstPersonCharacter::BindWeaponOnMesh(AWeaponBaseActor* WeaponToBind)
{
	if (GetRemoteRole() == ROLE_Authority)
	{
		WeaponToBind->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GripPoint"));
		WeaponToBind->AttachToComponent(ThirdPersonMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GripPoint"));

		/*if (GetLocalRole() == ROLE_AutonomousProxy)
		{
			WeaponToBind->AttachToComponent(FirstPersonMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GripPoint"));
		}
		else if (GetLocalRole() == ROLE_SimulatedProxy)
		{
			WeaponToBind->AttachToComponent(ThirdPersonMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GripPoint"));
		}*/
	}
}

// Called every frame
void AFirstPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (IsLocallyControlled())
	{
		if (OnOtherPlayerInfo.IsBound())
		{
			UE_LOG(LogTemp, Warning, TEXT("Client %s Bound"), *GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Client %s Not Bound"), *GetName());
		}
	}
	else
	{
		if (OnOtherPlayerInfo.IsBound())
		{
			UE_LOG(LogTemp, Warning, TEXT("Sever %s Bound"), *GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Sever %s Not Bound"), *GetName());
		}
	}*/
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
	PlayerInputComponent->BindAction(TEXT("Interaction"), IE_Pressed, this, &AFirstPersonCharacter::OpenTreasureChest);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &AFirstPersonCharacter::StartCrouch);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &AFirstPersonCharacter::EndCrouch);

}

float AFirstPersonCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	/*if (GetNetMode() == ENetMode::NM_DedicatedServer)
	{
		UE_LOG(LogTemp, Warning, TEXT("TakeDamage: NM_DedicatedServer"));
	}
	if (GetNetMode() == ENetMode::NM_Client)
	{
		UE_LOG(LogTemp, Warning, TEXT("TakeDamage: NM_Client"));
	}*/
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);
		if (Weapon)
		{
			OnGameInfoUIUpdate(CurrentHealth, MaxHealth, Weapon->AmmoNumInClip, Weapon->AmmoTotalNum, Weapon->Name);
		}
	}
	return 0.0f;
}

void AFirstPersonCharacter::AddControllerPitchInput(float Val)
{
	Super::AddControllerPitchInput(Val);

	if (IsLocallyControlled())
	{
		FRotator ControllerRotator = GetControlRotation();
		UpdateRotator(ControllerRotator);
	}
	/*else
	{
		FRotator ControllerRotator = GetControlRotation();
		UpdateRotatorMulticast(ControllerRotator);
	}*/
}

void AFirstPersonCharacter::UpdateRotatorMulticast_Implementation(FRotator NewRotator)
{
	FirstPersonCameraComponent->SetWorldRotation(NewRotator);
}

void AFirstPersonCharacter::UpdateRotator_Implementation(FRotator NewRotator)
{
	FirstPersonCameraComponent->SetWorldRotation(NewRotator);
	// ThirdPersonMesh->SetWorldRotation(NewRotator);
	GetController()->SetControlRotation(NewRotator);

	FRotator DeltaRotator = GetActorRotation() - GetControlRotation();
	DeltaRotator.Normalize();
	DeltaYaw = DeltaRotator.Yaw;
	DeltaPitch = -DeltaRotator.Pitch;

	UpdateRotatorMulticast(NewRotator);
}

void AFirstPersonCharacter::PlayFireMontageSoundMulticast_Implementation()
{
	if (FirstPersonFireAnimation)
	{
		PlayAnimMontage(FirstPersonFireAnimation);
		
	}
	UAnimInstance * AnimInstance = ThirdPersonMesh->GetAnimInstance(); 
    if( ThirdPersonFireAnimation && AnimInstance )
    {
	    AnimInstance->Montage_Play(ThirdPersonFireAnimation);
    }
}

void AFirstPersonCharacter::PlayReloadMontageSoundMulticast_Implementation()
{
	UAnimInstance * AnimInstance = ThirdPersonMesh->GetAnimInstance(); 
	if( ThirdPersonReloadAnimation && AnimInstance )
	{
		AnimInstance->Montage_Play(ThirdPersonReloadAnimation);
	}
}

void AFirstPersonCharacter::HandleReloadOnSever_Implementation()
{
	// Weapon->Reload();
	if (Weapon->AmmoTotalNum - Weapon->AmmoNumInClip > 0 && Weapon->AmmoNumInClip < Weapon->ClipMaxAmmo)
	{
		PlayReloadMontageSoundMulticast();
		PlayFirstPersonReloadMontageOnClient();
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, [this]() {
			if (Weapon->AmmoTotalNum < Weapon->ClipMaxAmmo - Weapon->AmmoNumInClip)
			{
				Weapon->AmmoNumInClip += Weapon->AmmoTotalNum;
				Weapon->AmmoTotalNum = 0;
			}
			else
			{
				Weapon->AmmoTotalNum -= Weapon->ClipMaxAmmo - Weapon->AmmoNumInClip;
				Weapon->AmmoNumInClip = Weapon->ClipMaxAmmo;
			}
			OnGameInfoUIUpdate(CurrentHealth, MaxHealth, Weapon->AmmoNumInClip, Weapon->AmmoTotalNum, Weapon->Name);
		}, 2.0f, false);
		UE_LOG(LogTemp, Warning, TEXT("%d"), Weapon->AmmoNumInClip);
	}
}

void AFirstPersonCharacter::OnGameInfoUIUpdate_Implementation(float CurrentHealthNow, float MaxHealthNow, int32 AmmoNumInClipNow, int32 AmmoTotalNumNow, const FString& NameNow)
{
	/*if (GetNetMode() == ENetMode::NM_DedicatedServer)
	{
		UE_LOG(LogTemp, Warning, TEXT("NM_DedicatedServer"));
	}
	if (GetNetMode() == ENetMode::NM_Client)
	{
		UE_LOG(LogTemp, Warning, TEXT("NM_Client"));
	}*/
	if (IsLocallyControlled())
	{
		if (OnUIChange.IsBound())
		{
			OnUIChange.Broadcast(CurrentHealthNow, MaxHealthNow,AmmoNumInClipNow, AmmoTotalNumNow, NameNow);
		}
	}
}

/*void AFirstPersonCharacter::OnRep_GameUI()
{
	UE_LOG(LogTemp, Warning, TEXT("AFirstPersonCharacter::OnRep_GameUI"));
	OnUIChange.Broadcast(CharacterInfo.CurrentHealth, CharacterInfo.MaxHealth,CharacterInfo.AmmoNumInClip, CharacterInfo.AmmoTotalNum, CharacterInfo.WeaponName);
}*/

void AFirstPersonCharacter::BeginGameInfoUpdateOnServer_Implementation()
{
	/*if (GetNetMode() == ENetMode::NM_DedicatedServer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Test NM_DedicatedServer"));
	}
	if (GetNetMode() == ENetMode::NM_Client)
	{
		UE_LOG(LogTemp, Warning, TEXT("Test NM_Client"));
	}*/
	OnGameInfoUIUpdate(CurrentHealth, MaxHealth, Weapon->AmmoNumInClip, Weapon->AmmoTotalNum, Weapon->Name);
	OnOtherPlayerInfo.ExecuteIfBound(CurrentHealth, MaxHealth, GetName());
}

void AFirstPersonCharacter::PlayFirstPersonReloadMontageOnClient_Implementation()
{
	UAnimInstance * AnimInstance = GetMesh()->GetAnimInstance(); 
	if( FirstPersonReloadAnimation && AnimInstance )
	{
		AnimInstance->Montage_Play(FirstPersonReloadAnimation);
	}
}

void AFirstPersonCharacter::FillAmmoOnServer_Implementation()
{
	Weapon->FillAmmo();
}

void AFirstPersonCharacter::OnRep_GoldNum()
{
	if (IsLocallyControlled())
	{
		if (OnGoldAdd.IsBound())
		{
			OnGoldAdd.Broadcast(GoldNum);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("OnGoldAdd not be bound"));
		}
	}
}

void AFirstPersonCharacter::OnRep_CurrentHealth()
{
	if (OtherCharacter)
	{
		if (ACharacterPlayerState* PS = OtherCharacter->GetPlayerState<ACharacterPlayerState>())
		{
			// PS->OtherPlayerCurrentHealth = CurrentHealth;
			PS->ChangeOtherPlayerCurrentHealthOnSever(CurrentHealth);
		}
	}
}

void AFirstPersonCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFirstPersonCharacter, DeltaYaw);
	DOREPLIFETIME(AFirstPersonCharacter, DeltaPitch);
	DOREPLIFETIME(AFirstPersonCharacter, GoldNum);
	DOREPLIFETIME(AFirstPersonCharacter, CurrentHealth);
}

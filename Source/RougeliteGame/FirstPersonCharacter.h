// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/World.h"
#include "GameFramework/Character.h"

#include "FirstPersonCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FUIChange, float, CurrentHealth, float, MaxHealth, int32, AmmoNumInClip, int32, AmmoTotalNum, FString, WeaponName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGoldNum, int32, GoldCount);

UCLASS()
class ROUGELITEGAME_API AFirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()
	
	/** Pawn mesh: 1st person view (arms; seen only by self) */
    UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
    class USkeletalMeshComponent* Mesh1P;
    
    // Sets default values for this character's properties
    	AFirstPersonCharacter();

public:
	/** First person camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FirstPersonCameraComponent;
	
	// Weapon actor, gun0 by default
	UPROPERTY(VisibleDefaultsOnly, Category=Weapon)
	class AWeaponBaseActor* Weapon;
	
	UPROPERTY(VisibleDefaultsOnly)
	TArray<class AWeaponBaseActor*> WeaponArray;

	// the index of weapon that we using
	UPROPERTY(VisibleDefaultsOnly)
	int8 WeaponIndex;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* ReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CurrentHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAim;

	UPROPERTY(VisibleDefaultsOnly)
	class ARougeliteGameGameModeBase* RougeliteGameGameMode;

	UPROPERTY(VisibleDefaultsOnly)
	class UParticleSystemComponent* ParticleComponent;

	UPROPERTY(VisibleDefaultsOnly)
	bool bBurning;

	UPROPERTY(VisibleDefaultsOnly)
	bool bOverlappingTreasureChest;

	UPROPERTY(VisibleDefaultsOnly)
	class ATreasureChest* TreasureChest;

	UPROPERTY(VisibleDefaultsOnly)
	int32 GoldNum;

	UPROPERTY(BlueprintAssignable)
	FUIChange OnUIChange;

	UPROPERTY(BlueprintAssignable)
	FGoldNum OnGoldAdd;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	// Fire
	void Fire();

	void Reload();

	void SwitchWeapon();

	void StartSprint();

	void StopSprint();

	void StartAim();

	void StopAim();

	void Open();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};

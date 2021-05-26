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
    class USkeletalMeshComponent* FirstPersonMesh;
    
    // Sets default values for this character's properties
    	AFirstPersonCharacter();

public:
	/** First person camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FirstPersonCameraComponent;

	/** Pawn mesh: 3st person view (arms; seen only by others) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	class USkeletalMeshComponent* ThirdPersonMesh;
	
	// Weapon actor
	UPROPERTY(VisibleDefaultsOnly, Category=Weapon)
	class AWeaponBaseActor* Weapon;
	
	UPROPERTY(VisibleDefaultsOnly)
	TArray<class AWeaponBaseActor*> WeaponArray;

	/*// Weapon actor, copy real weapon, only owner can see
	UPROPERTY(VisibleDefaultsOnly, Category=Weapon)
	class AWeaponBaseActor* WeaponFirstPerson;
	
	UPROPERTY(VisibleDefaultsOnly)
	TArray<class AWeaponBaseActor*> WeaponArrayFirstPerson;

	// Weapon actor, copy real weapon to show in client, only other players can see
	UPROPERTY(VisibleDefaultsOnly, Category=Weapon)
	class AWeaponBaseActor* WeaponThirdPerson;
	
	UPROPERTY(VisibleDefaultsOnly)
	TArray<class AWeaponBaseActor*> WeaponArrayThirdPerson;*/

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

	UPROPERTY(BlueprintReadOnly, Replicated)
	float DeltaYaw;

	UPROPERTY(BlueprintReadOnly, Replicated)
	float DeltaPitch;
	
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

	virtual void AddControllerPitchInput(float Val) override;

	UFUNCTION(Server, Reliable)
	void UpdateRotator(FRotator NewRotator);

	UFUNCTION(NetMulticast, Reliable)
	void UpdateRotatorMulticast(FRotator NewRotator);

	UFUNCTION(Server, Reliable)
    void HandleFireOnServer();

	UFUNCTION(Server, Reliable)
    void HandleSwitchWeaponOnSever();

	UFUNCTION(NetMulticast, Unreliable)
	void PlayMontageMulticast();

	UFUNCTION(NetMulticast, Reliable)
	void SetWeaponVisible(class AWeaponBaseActor* WeaponToBeHidden);

	UFUNCTION(Client, Reliable)
	void SetWeaponVisibleOnClient(class AWeaponBaseActor* WeaponToBeHidden);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

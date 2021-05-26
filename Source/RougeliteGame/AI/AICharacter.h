// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

UCLASS()
class ROUGELITEGAME_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleDefaultsOnly)
	class USkeletalMeshComponent* AIMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CurrentHealth)
	float CurrentHealth;

	UPROPERTY(VisibleDefaultsOnly)
	class UAnimationAsset* DeathAnimation;

	UPROPERTY(VisibleDefaultsOnly)
	class UWidgetComponent* HealthBarComponent;

	UPROPERTY(VisibleDefaultsOnly)
	class AWeaponBaseActor* Weapon;

	UPROPERTY(VisibleDefaultsOnly)
	float HeadLowBoundZ;

	UPROPERTY(EditAnywhere)
	int32 GoldReward;
	
	UPROPERTY(EditAnywhere)
	int32 FireAmmoNum;

	int32 FireAmmoCount;

	FTimerHandle FireHandle;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	bool bAttacked;

	/** Event for taking damage. Overridden from APawn.*/
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION(BlueprintCallable)
	void UpdateWalkSpeed(float Speed);

	UFUNCTION(BlueprintCallable)
	void Fire();

	void FireOnce();

	/** RepNotify for changes made to current health.*/
	UFUNCTION()
	void OnRep_CurrentHealth();

	/** Response to health being updated. Called on the server immediately after modification, and on clients in response to a RepNotify*/
	void OnHealthUpdate();

	/** Setter for Current Health. Clamps the value between 0 and MaxHealth and calls OnHealthUpdate. Should only be called on the server.*/
	void SetCurrentHealth(float HealthValue);

	/** Property replication */
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

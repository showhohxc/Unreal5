// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Characters/DefineCharacterTypes.h"
#include "AllCharacterBase.generated.h"

UCLASS()
class SLASH_API AAllCharacterBase : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAllCharacterBase();
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() { return DeathPose; }
protected:	
	virtual void BeginPlay() override;
	
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void Attack();
	virtual void Die();
	virtual void HandleDamage(float DamageAmount);
	virtual bool CanAttack();
	void DirectionalHitReact(const FVector& ImpactPoint);
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);
	void DisableCapsule();
	bool IsAlive();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	UFUNCTION(BlueprintCallable)
	virtual void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	//Montage ==================================================
	void PlayMontageSection(UAnimMontage* Montage, const FName& SelectName);
	void PlayHitReactMontage(const FName& strName);
	virtual int PlayComboAttackMontage();
	virtual int PlayRandomAttackMontage();
	virtual int PlayDeathMontage();
	void StopAttackMontage();

	UFUNCTION(BlueprintCallable)
	virtual FVector GetTransWarpTarget();

	UFUNCTION(BlueprintCallable)
	virtual FVector GetRotationWarpTarget();

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class AWeapon* EquippedWeapon;

	//===========================================================

	// Components ===============================================

	UPROPERTY(VisibleAnywhere)
	class UAttributeComponent* Attributes;

	UPROPERTY(BlueprintReadOnly, Category = Combat)
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = Combat)
	double doubleWarpTargetDistance = 75.0f;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;

	//===========================================================

	// Animation Montage (protected) ===============================================
	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* AttackMontage;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> AttackMontageSections;

private:
	int PlayComboMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);
	int PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	UPROPERTY(EditAnywhere, Category = Combat)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = Combat)
	UParticleSystem* HitParticles;

	// Animation Montage ===============================================

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* HitReactMontages;

	UPROPERTY(EditDefaultsOnly, Category = Combat)
	UAnimMontage* DeathMontages;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> DeathMontageSections;

	//==================================================================
};

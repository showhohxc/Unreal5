// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AllCharacterBase.h"
#include "Characters/DefineCharacterTypes.h"
#include "Components/SphereComponent.h"
#include "BaseCharacter/CustomSkillStruct.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API APlayerCharacter : public AAllCharacterBase
{
	GENERATED_BODY()
	
public:
	APlayerCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void PossessedBy(AController* NewController) override;
protected:
	UPROPERTY(EditAnywhere, Category = HUD)
	class AMainHUD* MainHUD;

	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* SkillMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> SkillMontageSections;

	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* DogeMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	AActor* SkillTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	bool bTargetLock = false;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void MoveForward(float value);
	void MoveRight(float value);
	void Turn(float value);
	void LookUp(float value);
	void EKeyPressed();
	virtual void Attack() override;
	void Doge();

	// 캐릭터 몽타주 섹션
	virtual void AttackEnd() override;
	virtual bool CanAttack() override;
	bool CanDisArm();
	bool CanArm();
	void Arm();
	void PlayEquipMontages(const FName& SectionName);
	void PlayDogeMontage();
	virtual void Die() override;

	UFUNCTION(BlueprintCallable)
	void TargetLockOn();

	UFUNCTION(BlueprintCallable)
	void TargetLockOFF();

	UFUNCTION(BlueprintCallable)
	void DogeEnd();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	UFUNCTION(BlueprintCallable)
	void StartMotion();

	// Attack Sequence
	UFUNCTION(BlueprintCallable)
	void AttackSaveComb();

	UFUNCTION(BlueprintCallable)
	void ResetComb();

	UFUNCTION(BlueprintCallable)
	void ImpulseAttack();

	UFUNCTION(BlueprintCallable)
	void SkillQ_Move();

	virtual FVector GetTransWarpTarget() override;
	virtual FVector GetRotationWarpTarget() override;
	
private:
	bool IsUnoccupied();
	void InitUI();;
	void SetHUDHealth();
	void ProcPlayerCombatCombo();
	void Skill_Q();
	void Skill_E();
	void Skill_R();
	void ProcSkillActive(int nIndex);

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, Category = Skill)
	USphereComponent* SkillSphereRadius;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	class UAnimMontage* EquipMontage;

	ECharacterState CharacterState = ECharacterState::ECS_Uniquipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY()
	class USlashOverlay* SlashOverlay;

	UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(VisibleAnywhere, Category = Animation)
	class UAnimInstance* PlayerAnimInstance;

	// Montage
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* StartMontage;
	//


	bool m_bIsAttacking = false;
	bool m_bSaveAttack = false;
	int m_nAttackCount = 0;

	UPROPERTY(EditAnywhere, Category = Combat, Meta = (AllowPrivateAccess = true))
	float AttackMoveImpulse;//미세전진에 가할 힘

	UPROPERTY(EditAnywhere, Category = Skill)
	float SkillQMagnitude = 2500.0f;

	UPROPERTY(EditAnywhere, Category = Combat)
	FCustomSkillStruct SkillStruct;

	UPROPERTY()
	class UPlayerSkilOverlay* PlayerSkillOverlay;

	UPROPERTY()
	class APlayerController* PlayerController;

	UPROPERTY(EditAnywhere, Category = Combat)
	bool bShowBoxDebug = false;

	UPROPERTY()
	class AEnemy* LockOnEnemy;
public:
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
	FORCEINLINE void SetActionState(EActionState actionState) { ActionState = actionState; }

	FORCEINLINE TArray<bool> GetPlayerAttack() const { return SkillStruct.bPlayerDefaultAttackCombo; }
	FORCEINLINE void SetPlayerAttack(int nNum, bool DefaultAttackUnLock) { SkillStruct.bPlayerDefaultAttackCombo[nNum] = DefaultAttackUnLock; }

	FORCEINLINE TArray<bool> GetPlayerSkill() const { return SkillStruct.bPlayerSkillUnLock; }
	FORCEINLINE void SetPlayerSkill(int nNum, bool DefaultSkillUnLock) { SkillStruct.bPlayerSkillUnLock[nNum] = DefaultSkillUnLock; }

	FORCEINLINE AActor* GetPlayerSkillTarget() const { return SkillTarget; }

	FORCEINLINE bool GetPlayerTargetActive() const { return bTargetLock; }
};

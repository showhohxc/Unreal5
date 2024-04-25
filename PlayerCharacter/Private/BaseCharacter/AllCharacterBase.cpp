// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter/AllCharacterBase.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "items/Weapons/Weapon.h"
#include "Components/AttributeComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Slash/Public/AllMacro.h"

AAllCharacterBase::AAllCharacterBase()
{ 	
	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void AAllCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAllCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AAllCharacterBase::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (IsAlive() && Hitter)
		DirectionalHitReact(Hitter->GetActorLocation());
	else
		Die();

	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}

void AAllCharacterBase::Attack()
{
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead")))
	{
		CombatTarget = nullptr;
	}
}

void AAllCharacterBase::Die()
{
	Tags.Add(FName("Dead"));
	PlayDeathMontage();
}

void AAllCharacterBase::HandleDamage(float DamageAmount)
{
	if (Attributes)
		Attributes->ReceiveDamage(DamageAmount);
}

bool AAllCharacterBase::CanAttack()
{
	return false;
}

void AAllCharacterBase::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// 두 벡터사이의 각도 획득 
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// 세타값 획득 * 내적 후 Acos을 통한 각도 획득
	double Theta = FMath::Acos(CosTheta);
	// Radian 을 통한 각도 획득
	Theta = FMath::RadiansToDegrees(Theta);

	// 외적으로 표면벡터를 통해 Z 값으로 양수 음수 구분
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);

	if (CrossProduct.Z < 0)
		Theta *= -1.0f;

	FName Section("FromBack");

	if (Theta >= -45.0f && Theta < 45.0f)
		Section = FName("FromFront");
	else if (Theta >= -135.0f && Theta < -45.0f)
		Section = FName("FromLeft");
	else if (Theta >= 45.0f && Theta < 135.0f)
		Section = FName("FromRight");

	PlayHitReactMontage(Section);
}

void AAllCharacterBase::PlayHitSound(const FVector& ImpactPoint)
{
	UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
}

void AAllCharacterBase::SpawnHitParticles(const FVector& ImpactPoint)
{
	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, ImpactPoint);
	}
}

void AAllCharacterBase::DisableCapsule()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool AAllCharacterBase::IsAlive()
{
	return Attributes->IsAlive();
}

void AAllCharacterBase::AttackEnd()
{
}

void AAllCharacterBase::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnorActors.Empty();
	}
}

void AAllCharacterBase::PlayHitReactMontage(const FName& strName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && HitReactMontages)
	{
		AnimInstance->Montage_Play(HitReactMontages);
		AnimInstance->Montage_JumpToSection(strName, HitReactMontages);
	}
}

int AAllCharacterBase::PlayComboAttackMontage()
{
	return PlayComboMontageSection(AttackMontage, AttackMontageSections);;
}

int AAllCharacterBase::PlayComboMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if (SectionNames.Num() <= 0) return -1;

	const int MaxSetionIndex = SectionNames.Num() - 1;
	const int Selection = FMath::RandRange(0, MaxSetionIndex);
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}

int AAllCharacterBase::PlayRandomAttackMontage()
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

void AAllCharacterBase::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0.25f, AttackMontage);
	}
}

int AAllCharacterBase::PlayDeathMontage()
{
	const int Selection = PlayRandomMontageSection(DeathMontages, DeathMontageSections);
	
	TEnumAsByte<EDeathPose> Pose(Selection);

	if (Pose < EDeathPose::EDP_MAX)
	{
		DeathPose = Pose;
	}

	return Selection;
}

int AAllCharacterBase::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if (SectionNames.Num() <= 0) return -1;

	const int MaxSetionIndex = SectionNames.Num() - 1;
	const int Selection = FMath::RandRange(0, MaxSetionIndex);
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}

void AAllCharacterBase::PlayMontageSection(UAnimMontage* Montage, const FName& SelectName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();	// ABP_Character

	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SelectName, Montage);
	}
}

FVector AAllCharacterBase::GetTransWarpTarget()
{
	if (CombatTarget == nullptr)
		return FVector();

	const FVector EnemyTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();

	FVector TargetToMe = (Location - EnemyTargetLocation).GetSafeNormal();
	TargetToMe *= doubleWarpTargetDistance; 

	return EnemyTargetLocation + TargetToMe;
}

FVector AAllCharacterBase::GetRotationWarpTarget()
{
	if (CombatTarget)
		return CombatTarget->GetActorLocation();

	return FVector();
}


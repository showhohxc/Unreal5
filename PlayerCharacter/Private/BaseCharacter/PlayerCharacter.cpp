#include "BaseCharacter/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/AttributeComponent.h"
#include "items/ItemBase.h"
#include "items/Weapons/Weapon.h"
#include "Enemy/Enemy.h"
#include "Animation/AnimMontage.h"
#include "HUD/MainHUD.h"
#include "HUD/PlayerSkilOverlay.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Vector.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 400.0f, 0);

	/** Box Overlap / Box Trace */
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	SkillSphereRadius = CreateDefaultSubobject<USphereComponent>(TEXT("SkillSphereRadius"));
	SkillSphereRadius->SetupAttachment(GetRootComponent());
	SkillSphereRadius->SetGenerateOverlapEvents(true);
	//////////////////////////////////////////////////////////////////////////

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 550.0f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraCoponent"));
	CameraComponent->SetupAttachment(SpringArm);
	CameraComponent->SetRelativeLocation(FVector(0, 0, 200.0f));
	CameraComponent->SetRelativeRotation(FRotator(-10.0f, 0, 0));
	
	SkillStruct.bPlayerDefaultAttackCombo.Init(false, 3);
	SkillStruct.bPlayerSkillUnLock.Init(false, 3);

	bTargetLock = false;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());
	UE_LOG(LogTemp, Warning, TEXT("GetController %s"), *GetController()->GetName());
	UE_LOG(LogTemp, Warning, TEXT("APlayerController %s"), *PlayerController->GetName());
	MainHUD = Cast<AMainHUD>(PlayerController->GetHUD());

	PlayerAnimInstance = GetMesh()->GetAnimInstance();
	Tags.Add(FName("EngageableTarget"));

	SkillSphereRadius->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnSphereOverlap);
	SkillSphereRadius->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnSphereEndOverlap);

	UWorld* World = GetWorld();

	if (World && WeaponClass)
	{
		AWeapon* Sword = World->SpawnActor<AWeapon>(WeaponClass);
		Sword->Equip(GetMesh(), FName("RightHandSocket"), this, this, false);
		EquippedWeapon = Sword;
	}

	ActionState = EActionState::EAS_StartStandMotion;
	PlayerAnimInstance->Montage_Play(StartMontage);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LockOnEnemy != nullptr)
	{
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockOnEnemy->GetActorLocation());
		PlayerController->SetControlRotation(rot);
	}
}


void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &APlayerCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &APlayerCharacter::LookUp);

	// Action
	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction(FName("Equip"), IE_Pressed, this, &APlayerCharacter::EKeyPressed);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &APlayerCharacter::Attack);
	PlayerInputComponent->BindAction(FName("Doge"), IE_Pressed, this, &APlayerCharacter::Doge);
	
	PlayerInputComponent->BindAction(FName("Skill_Q"), IE_Pressed, this, &APlayerCharacter::Skill_Q);
	PlayerInputComponent->BindAction(FName("Skill_E"), IE_Pressed, this, &APlayerCharacter::Skill_E);
	PlayerInputComponent->BindAction(FName("Skill_R"), IE_Pressed, this, &APlayerCharacter::Skill_R);
	
}

void APlayerCharacter::Jump()
{
	if (ActionState == EActionState::EAS_Unoccupied)
	{
		SkillStruct.bPlayerDefaultAttackCombo[0] = true;
		Super::Jump();
	}
}

float APlayerCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	return 0.0f;
}

void APlayerCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;
}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

}


/*
* Controller->GetControlRotation(); 현재 Rotaiton 값 get
* FRotationMaxtrix 을 통해 회전행렬로 값을 변환
* 변환된 회전행렬에서 원하는 Axis(축)을 구한다 X : 전 후방 Y : 좌 우측
*/
void APlayerCharacter::MoveForward(float value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;

	if ((Controller != nullptr) && (value != 0.0f))
	{
		const FRotator ControlRotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, ControlRotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);
	}
}

void APlayerCharacter::MoveRight(float value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;

	if ((Controller != nullptr) && (value != 0.0f))
	{
		const FRotator ControlRotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, ControlRotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, value);
	}
}

void APlayerCharacter::Turn(float value)
{
	AddControllerYawInput(value);
}

void APlayerCharacter::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void APlayerCharacter::EKeyPressed()
{

}

void APlayerCharacter::Attack()
{
	if (ActionState == EActionState::EAS_StartStandMotion || ActionState == EActionState::EAS_PlayerOption) return;

	Super::Attack();

	if (ActionState == EActionState::EAS_Unoccupied)
	{
		ActionState = EActionState::EAS_Attacking;
	}

	if (PlayerAnimInstance && AttackMontage)
	{
		if (AttackMontageSections.Num() == 0) return;

		if (m_bIsAttacking != true)
		{
			m_bIsAttacking = true;
			//UE_LOG(LogTemp, Warning, TEXT("Attack : nAttackCount : %d"), m_nAttackCount);
			ProcPlayerCombatCombo();
		}
		else
		{
			m_bSaveAttack = true;
		}
	}
}

void APlayerCharacter::Doge()
{
	if (ActionState != EActionState::EAS_Unoccupied)
		return;

	PlayDogeMontage();
	
	SetActorRotation(UKismetMathLibrary::MakeRotFromX(GetLastMovementInputVector()));
	ActionState = EActionState::EAS_Doge;
}

void APlayerCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool APlayerCharacter::CanAttack()
{
	return false;
}

bool APlayerCharacter::CanDisArm()
{
	return false;
}

bool APlayerCharacter::CanArm()
{
	return false;
}

void APlayerCharacter::Arm()
{
}

void APlayerCharacter::PlayEquipMontages(const FName& SectionName)
{
}

void APlayerCharacter::PlayDogeMontage()
{
	PlayMontageSection(DogeMontage, FName("Default"));
}

void APlayerCharacter::Die()
{
	ActionState = EActionState::EAS_Dead;
}

void APlayerCharacter::TargetLockOn()
{
	FVector Start = GetActorLocation();
	FVector End = (CameraComponent->GetForwardVector() * 500.0f) + Start;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.AddUnique(GetOwner());

	UE_LOG(LogTemp, Warning, TEXT("TargetLockOn | Start : %s \n End : %s"), *Start.ToString(), *End.ToString());

	FHitResult OutHit;
	FLinearColor TraceColor = FLinearColor::Red;
	FLinearColor TraceHitColor = FLinearColor::Green;

		UKismetSystemLibrary::SphereTraceSingleForObjects(this, Start, End, 125.0f, ObjectTypesArray,
			false,
			ActorsToIgnore,
			bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
			OutHit,
			true
			);

		//UE_LOG(LogTemp, Warning, TEXT("OutHit : %s"), *OutHit.GetActor()->GetName());

		if (OutHit.GetActor())
		{
			LockOnEnemy = Cast<AEnemy>(OutHit.GetActor());

			if (LockOnEnemy)
			{
				bTargetLock = true;
				PlayerController->SetIgnoreLookInput(true);
				this->GetCharacterMovement()->bOrientRotationToMovement = false;
				this->GetCharacterMovement()->bUseControllerDesiredRotation = true;
			}
		}
}

void APlayerCharacter::TargetLockOFF()
{
	bTargetLock = false;
	LockOnEnemy = nullptr;
	PlayerController->SetIgnoreLookInput(false);
	this->GetCharacterMovement()->bOrientRotationToMovement = true;
	this->GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

void APlayerCharacter::DogeEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void APlayerCharacter::FinishEquipping()
{
}

void APlayerCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void APlayerCharacter::StartMotion()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool APlayerCharacter::IsUnoccupied()
{
	return false;
}

void APlayerCharacter::InitUI()
{
}

void APlayerCharacter::SetHUDHealth()
{
}

void APlayerCharacter::AttackSaveComb()
{
	if (m_bSaveAttack)
	{
		m_bSaveAttack = false;
		ProcPlayerCombatCombo();
	}
}

void APlayerCharacter::ResetComb()
{
	m_nAttackCount = 0;
	m_bSaveAttack = false;
	m_bIsAttacking = false;
}

void APlayerCharacter::ImpulseAttack()
{
	this->GetCharacterMovement()->AddImpulse(GetActorForwardVector() * AttackMoveImpulse, true);
}

void APlayerCharacter::SkillQ_Move()
{
	FVector Impurse = GetActorForwardVector() * SkillQMagnitude;
	GetCharacterMovement()->AddImpulse(Impurse, true);
}

void APlayerCharacter::ProcPlayerCombatCombo()
{
	switch (m_nAttackCount)
	{
		case 0:
		{
			PlayerAnimInstance->Montage_Play(AttackMontage);
			PlayerAnimInstance->Montage_JumpToSection(AttackMontageSections[m_nAttackCount]);
			++m_nAttackCount;
		} break;

		case 1:
		{
			PlayerAnimInstance->Montage_Play(AttackMontage);
			PlayerAnimInstance->Montage_JumpToSection(AttackMontageSections[m_nAttackCount]);
			++m_nAttackCount;
		} break;

		case 2:
		{
			PlayerAnimInstance->Montage_Play(AttackMontage);
			PlayerAnimInstance->Montage_JumpToSection(AttackMontageSections[m_nAttackCount]);

			if (SkillStruct.bPlayerDefaultAttackCombo[0])
				++m_nAttackCount;
			else
				m_nAttackCount = 0;
		} break;
		case 3:
		{
			PlayerAnimInstance->Montage_Play(AttackMontage);
			PlayerAnimInstance->Montage_JumpToSection(AttackMontageSections[m_nAttackCount]);

			if (SkillStruct.bPlayerDefaultAttackCombo[1])
				++m_nAttackCount;
			else
				m_nAttackCount = 0;
		} break;
		case 4:
		{
			PlayerAnimInstance->Montage_Play(AttackMontage);
			PlayerAnimInstance->Montage_JumpToSection(AttackMontageSections[m_nAttackCount]);

			if (SkillStruct.bPlayerDefaultAttackCombo[2])
				++m_nAttackCount;
			else
				m_nAttackCount = 0;
		} break;
	}
}

void APlayerCharacter::Skill_Q()
{
	ProcSkillActive(0);
}

void APlayerCharacter::Skill_E()
{
	if (SkillTarget != nullptr)
	{

		//PlayerController->SimpleMoveTo()

		//PlayerController->MoveTo(MoveRequest);

		ProcSkillActive(1);
	}
}

void APlayerCharacter::Skill_R()
{
	ProcSkillActive(2);
}

void APlayerCharacter::ProcSkillActive(int nIndex)
{
	if (SkillStruct.bPlayerSkillUnLock[nIndex] != true)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not UnLock"));
	}
	else
	{
		if (ActionState == EActionState::EAS_Unoccupied)
		{
			ActionState = EActionState::EAS_Attacking;
			PlayerAnimInstance->Montage_Play(SkillMontage);
			PlayerAnimInstance->Montage_JumpToSection(SkillMontageSections[nIndex]);
		}
	}
}

void APlayerCharacter::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter OnSphereOverlap OverlappedComponent Name : %s"), *OverlappedComponent->GetName());
	UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter OnSphereOverlap OhterActor Name : %s"), *OtherActor->GetName());*/

	AEnemy* Enemy = Cast<AEnemy>(OtherActor);

	if (Enemy)
	{
		SkillTarget = Enemy;
		//UE_LOG(LogTemp, Warning, TEXT("OnSphereOverlap TargetName %s : "), *Enemy->GetName());

		if (SkillStruct.bPlayerSkillUnLock[1])
			MainHUD->SkillEOpacity(1.0f);
	}
}

void APlayerCharacter::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	/*UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter OnSphereEndOverlap OverlappedComponent Name : %s"), *OverlappedComponent->GetName());
	UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter OnSphereEndOverlap OhterActor Name : %s"), *OtherActor->GetName());
	UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter OnSphereEndOverlap OtherComp Name : %s"), *OtherComp->GetName());*/

	AEnemy* Enemy = Cast<AEnemy>(OtherActor);

	if (Enemy)
	{
		if (SkillTarget)
		{
			SkillTarget = nullptr;

			//UE_LOG(LogTemp, Warning, TEXT("OnSphereEndOverlap TargetName Null "));

			if (SkillStruct.bPlayerSkillUnLock[1])
				MainHUD->SkillEOpacity(0.2f);
		}
	}
}

FVector APlayerCharacter::GetTransWarpTarget()
{
	if (SkillTarget == nullptr)
		return FVector();

	const FVector EnemyTargetLocation = SkillTarget->GetActorLocation();
	const FVector Location = GetActorLocation();

	FVector TargetToMe = (Location - EnemyTargetLocation).GetSafeNormal();
	TargetToMe *= doubleWarpTargetDistance;

	/*UE_LOG(LogTemp, Warning, TEXT("EnemyTargetLocation SkillTarget %s"), *SkillTarget->GetActorLocation().ToString());
	UE_LOG(LogTemp, Warning, TEXT("EnemyTargetLocation MyLocation %s"), *GetActorLocation().ToString());*/

	return EnemyTargetLocation + TargetToMe;
}

FVector APlayerCharacter::GetRotationWarpTarget()
{
	//UE_LOG(LogTemp, Warning, TEXT("GetRotationWarpTarget Target %s"), *SkillTarget->GetName());

	if (SkillTarget)
		return SkillTarget->GetActorLocation();

	return FVector();
}


//APlayerController* PlayerController = Cast<APlayerController>(GetController());
//AMainHUD* MainHUD = Cast<AMainHUD>(PlayerController->GetHUD());
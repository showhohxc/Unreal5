### 1. Player Guard System<br>
![Guard-ezgif com-video-to-gif-converter](https://github.com/showhohxc/Unreal5/assets/98040028/e81b90b8-153b-4d7d-9c66-621ee945ab1e)<br/>

![image](https://github.com/showhohxc/Unreal5/assets/98040028/c81552ef-933f-4f6b-8d62-bed7e702263f)
*AnimGraph Guard State 추가  <br/>
![image](https://github.com/showhohxc/Unreal5/assets/98040028/a4f5551f-93ae-4122-8013-de5938712850)
*Guard 행동 유지를 위하여 Guard Pose를 포즈 블렌딩  <br/>

<strong> Player의 Guard State 추가에 따라 PlayerAnimInstance에 Guard 행동시에 PlayerAnimation 상태값 변경을 위하여 bGuardState 작성  <br/>
<strong> PlayerAnimInstance 에서 플레이어의 가드 상태값을 알기위하여 PlayerCharacter의 bGuardState를 인라인 함수로 작성  <br/>
```
PlayerCharacter.h
protected:
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
bool bGuardState = false;

public:
FORCEINLINE bool GetPlayerGuardState() const { return bGuardState; }


PlayerCharacter.c

void APlayerCharacter::GuardOn()
{
	ActionState = EActionState::EAS_Guard;
	PlayMontageSection(GuardMontage, FName("Guard_Start"));

	bGuardState = true;
}

void APlayerCharacter::GuardOFF()
{
	if (ActionState == EActionState::EAS_Guard)
	{
		ActionState = EActionState::EAS_Unoccupied;
		PlayMontageSection(GuardMontage, FName("Guard_End"));
		bGuardState = false;
		m_bPlayerHit = false;
	}
}

PlayerAnimInstance.c

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (PlayerCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(PlayerCharacterMovement->Velocity);
		Direction = CalculateDirection(PlayerChracter->GetVelocity(), PlayerChracter->GetActorRotation());
		bIsFalling = PlayerCharacterMovement->IsFalling();
		CharacterState = PlayerChracter->GetCharacterState();
		ActionState = PlayerChracter->GetActionState();
		DeathPose = PlayerChracter->GetDeathPose();
		bTargetLock = PlayerChracter->GetPlayerTargetActive();
		bGuardState = PlayerChracter->GetPlayerGuardState();
	}
}


```

<strong> Player가 Hit 발생시 Guard 상태이고 Parry 상태가 아닐시 파티클과 사운드 생성  <br/>
```
void APlayerCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (ActionState == EActionState::EAS_Guard)
	{
		m_bPlayerHit = true;
		m_vecImpactPoint = ImpactPoint;

		if(ActionState != EActionState::EAS_Parry)
			SpawnGuardParticles(ImpactPoint, GuardParticles, HitBlockSound);
	}

	if (ActionState != EActionState::EAS_Unoccupied) 
		return;
}
```

### 2. Player Parry System<br>
![Parry-ezgif com-video-to-gif-converter](https://github.com/showhohxc/Unreal5/assets/98040028/383e2749-af3d-4a33-9bad-50a867641272)<br/>
<strong> Player의 Parry State 추가 <br/>

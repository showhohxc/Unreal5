
### 1. Perfect Doge<br>
![PerfectDoge-ezgif com-video-to-gif-converter](https://github.com/showhohxc/Unreal5/assets/98040028/5597918f-0d68-4fb9-b1e9-896f4440f60c) <br/>

<strong> Player의 Doge 액션 중 GetHit 시 발생하는 ImpactPoint와 Player의 ActorLocation 사이의 위치 값을 계산하여 m_bPerfectDogeActive = true 발생 시 Perfect_Doge 시스템 활성화 <br/>
```
void APlayerCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (ActionState == EActionState::EAS_Doge)
	{
		const double DistanceCheck = (ImpactPoint - GetActorLocation()).Size();

		if (DistanceCheck <= 50.0f)
		{
			m_bPerfectDogeActive = true;
		}
	}

	if (ActionState != EActionState::EAS_Unoccupied) 
		return;
}
```

![image](https://github.com/showhohxc/Unreal5/assets/98040028/d4ec1c54-3188-4cca-b7c4-5a2a670e8cb7)

<strong> m_bPerfectDogeActive가 true로 활성화 되면서 Doge 몽타주에 PerfectDogeStart 와 PerfectDogeEnd에 대해 설정한 Notify가 조건에 해당하여 활성화
<strong> 동시에 SetGlobalTimeDilation(GetWorld(), fDeltaTime)과 나이아가라 Trail Effect를 활성화 후 PerfectDogeEnd시 정상화
```
void APlayerCharacter::PerfectDogeStart(float fDeltaTime)
{
	if (m_bPerfectDogeActive)
	{
		EffectInstance = UNiagaraFunctionLibrary::SpawnSystemAttached(TracerNS, GetMesh(), TEXT("BoneRoot"), FVector(0, 0, 0), FRotator(0, 0, 0),
				EAttachLocation::KeepWorldPosition, true);
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), fDeltaTime);
		CountAttackWidget->SetVisibility(true);
	}
}

void APlayerCharacter::PerfectDogeEnd(float fDeltaTime)
{
	if (m_bPerfectDogeActive)
	{
		if (EffectInstance)
			EffectInstance->DestroyComponent();
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), fDeltaTime);

		CountAttackWidget->SetVisibility(false);
	}
}

```


### 1. Counter Attack<br>
![CounterSkill-ezgif com-video-to-gif-converter](https://github.com/showhohxc/Unreal5/assets/98040028/55edab0d-9387-4fac-b2b8-4d991abe3456) <br/>

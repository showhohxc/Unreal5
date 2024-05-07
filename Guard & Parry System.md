### 1. Player Guard System<br>
![Guard-ezgif com-video-to-gif-converter](https://github.com/showhohxc/Unreal5/assets/98040028/e81b90b8-153b-4d7d-9c66-621ee945ab1e)<br/>
<strong> Player의 Guard State 추가 <br/>

'''
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
'''

### 2. Player Parry System<br>
![Parry-ezgif com-video-to-gif-converter](https://github.com/showhohxc/Unreal5/assets/98040028/383e2749-af3d-4a33-9bad-50a867641272)<br/>
<strong> Player의 Parry State 추가 <br/>

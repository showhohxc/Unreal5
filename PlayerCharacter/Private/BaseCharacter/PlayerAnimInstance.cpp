// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter/PlayerAnimInstance.h"
#include "BaseCharacter/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerChracter = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (PlayerChracter)
		PlayerCharacterMovement = PlayerChracter->GetCharacterMovement();
}

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
	}
}

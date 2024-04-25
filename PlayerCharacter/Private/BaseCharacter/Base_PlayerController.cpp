// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter/Base_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/MainHUD.h"
#include "BaseCharacter/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/PlayerSkilOverlay.h"

void ABase_PlayerController::BeginPlay()
{
	MainHUD = Cast<AMainHUD>(GetHUD());
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	UE_LOG(LogTemp, Warning, TEXT("ABase_PlayerController : PlayerCharacter %s "), *PlayerCharacter->GetName());
}

void ABase_PlayerController::OpenSkillBook()
{
	if (MainHUD)
	{
		PlayerSkillOverlay = MainHUD->GetPlayerSkillOverlay();

		if (PlayerSkillOverlay)
		{
			if (bPlayerSkillOverlay != true)
			{
				PlayerCharacter->SetActionState(EActionState::EAS_PlayerOption);
				bPlayerSkillOverlay = true;
				SetShowMouseCursor(true);
				SetInputMode(FInputModeGameAndUI());
				PlayerSkillOverlay->SetVisibility(ESlateVisibility::Visible);
				PlayerCharacter->GetCharacterMovement()->DisableMovement();
				SetIgnoreLookInput(true);
			}
			else
			{
				PlayerCharacter->SetActionState(EActionState::EAS_Unoccupied);
				bPlayerSkillOverlay = false;
				SetShowMouseCursor(false);
				SetInputMode(FInputModeGameOnly());
				PlayerSkillOverlay->SetVisibility(ESlateVisibility::Hidden);
				PlayerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
				ResetIgnoreLookInput();
			}
		}
	}
}
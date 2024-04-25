// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Base_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API ABase_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OpenSkillBook();

	UPROPERTY(BlueprintReadOnly)
	class APlayerCharacter* PlayerCharacter;

private:
	UPROPERTY()
	class AMainHUD* MainHUD;

	UPROPERTY()
	class UPlayerSkilOverlay* PlayerSkillOverlay;

	UPROPERTY()
	bool bPlayerSkillOverlay = false;
};

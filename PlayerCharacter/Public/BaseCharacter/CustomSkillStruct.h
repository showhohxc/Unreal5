// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "CustomSkillStruct.generated.h"

USTRUCT(Atomic, BlueprintType)
struct FCustomSkillStruct : public  FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<bool> bPlayerDefaultAttackCombo;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<bool> bPlayerSkillUnLock;
};
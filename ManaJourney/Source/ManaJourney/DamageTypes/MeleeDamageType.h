// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/DamageType.h"
#include "MeleeDamageType.generated.h"

/**
 * 
 */
UCLASS(const, Blueprintable, BlueprintType)
class MANAJOURNEY_API UMeleeDamageType : public UDamageType
{
	GENERATED_BODY()
	
public:
	UMeleeDamageType();

	// time elapse after the enemy receive the damage
	float fTakeDamageTime = 1.5f;
	
	
};

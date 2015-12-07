// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/DamageType.h"
#include "ManaDamageType.generated.h"

/**
 * 
 */
UCLASS()
class MANAJOURNEY_API UManaDamageType : public UDamageType
{
	GENERATED_BODY()
	
	
public:
	UManaDamageType();

	float fTakeDamageTime = 2.5f;
	
};

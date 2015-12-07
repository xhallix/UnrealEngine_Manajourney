// Fill out your copyright notice in the Description page of Project Settings.

#include "ManaJourney.h"
#include "BasicWeapon.h"



// Sets default values
ABasicWeapon::ABasicWeapon()
{
	
	MeeleCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeeleCapsule"));
	MeeleCapsule->AttachParent = GetStaticMeshComponent();
	
}


// Called when the game starts or when spawned
void ABasicWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame

void ABasicWeapon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}




void ABasicWeapon::setUserStrength(float level, AManaJourneyCharacter& character)
{
	character.strength = character.strength + strength;
}

void ABasicWeapon::setWeaponDamage(AManaJourneyCharacter& character)
{
	character.MeleeDamage = WeaponDamage;
}

void ABasicWeapon::handleAttackEffect(float effectTime, USkeletalMeshComponent* mesh, float attackingStateToUse)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("This is for overwriting purpose only")));
}

void ABasicWeapon::destroyEffect(UParticleSystemComponent* effect2Destroy)
{
	if (effect2Destroy)
	{
		effect2Destroy->DestroyComponent();
	}
}
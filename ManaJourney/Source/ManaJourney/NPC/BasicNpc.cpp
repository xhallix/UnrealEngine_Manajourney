// Fill out your copyright notice in the Description page of Project Settings.

#include "ManaJourney.h"
#include "BasicNpc.h"

// Sets default values
ABasicNpc::ABasicNpc(const FObjectInitializer& ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bSkippedTarget = false;

	// Create the TargetIndicator
	FocusComponent = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("TargetIndicator"));
	FocusComponent->AttachParent = GetMesh();
	FocusComponent->SetHiddenInGame(true);


}

// Called when the game starts or when spawned
void ABasicNpc::BeginPlay()
{
	Super::BeginPlay();
	bSkippedTarget = false;
}

// Called every frame
void ABasicNpc::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	// make the focus component move
	FocusComponent->AddWorldRotation(FRotator(0, 0.7, 0));
}

// Called to bind functionality to input
void ABasicNpc::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void ABasicNpc::toggleFocus()
{
	if (bFocused)
	{
		bFocused = false;
	}
	else
	{
		bFocused = true;
	}
}

void ABasicNpc::setFocus()
{
	bFocused = true;
}

void ABasicNpc::removeFocus()
{
	bFocused = false;
}

void ABasicNpc::showFocusComponent()
{
	FocusComponent->SetHiddenInGame(false);
}

void ABasicNpc::hideFocusComponent()
{
	FocusComponent->SetHiddenInGame(true);
}

void ABasicNpc::nextDialog()
{
	// restart dialog
	if (dialogState >= dialogTexts.Num())
	{
		dialogState = 0;
	}

	dialogText = dialogTexts[dialogState];
	FText textToShow = formatFText(dialogText);
	this->displayDialogText(textToShow);
	dialogState++;
}

void ABasicNpc::enableDialogSystem()
{
	// populate to blueprint widget
	if (dialogTexts.Num() != 0)
	{	
		this->enableDialogWidget();
		FText textToShow = formatFText(dialogTexts[0]);
		this->displayDialogText(textToShow);
		isDialogSystemEnabled = true;
	}
	
}

void ABasicNpc::disableDialogSystem()
{
	isDialogSystemEnabled = false;
	this->removeDialogWidget();
}

FText ABasicNpc::formatFText(const FText &textToFormat)
{
	FText textToReturn = FText::FromString(textToFormat.ToString());
	FString __lineBreak = "<br>";
	FString stringToFormat = textToFormat.ToString();
	if (stringToFormat.Contains(__lineBreak))
	{
		FString linebreakedString = stringToFormat.Replace(TEXT("<br>"), TEXT("\n"));
		textToReturn = FText::FromString(linebreakedString);
	}

	return textToReturn;
}
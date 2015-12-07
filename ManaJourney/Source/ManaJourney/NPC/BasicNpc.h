// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "BasicPassenger.h"
#include "BasicNpc.generated.h"

UCLASS()
class MANAJOURNEY_API ABasicNpc : public ABasicPassenger
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicNpc(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(Category = Private, VisibleDefaultsOnly)
		UStaticMeshComponent* FocusComponent;

	// all logic when beeing hit
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "DialogAttributes")
		TArray<FText>dialogTexts;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "enableDialogWidget"), Category = "DialogSystem")
		void enableDialogWidget();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "removeDialogWidget"), Category = "DialogSystem")
		void removeDialogWidget();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "displayDialogText", MultiLine = "true"), Category = "DialogSystem")
		void displayDialogText(const FText &dialogTextToDisplay);

	// focused by target system
	bool bFocused = false;
	// indicates if this enemy has been focused, so it can be skipped to select the next
	bool bSkippedTarget = false;
	bool bIsDead = false;
	bool isDialogSystemEnabled = false;

	// sets bFocused
	void toggleFocus();
	void setFocus();
	void removeFocus();
	void showFocusComponent();
	void hideFocusComponent();
	

	// enbaleDialogSystem MUST be called before next dialog
	void enableDialogSystem();
	void disableDialogSystem();
	void nextDialog();
	
	// pretty sure this should better be placed in some utils functions
	// since this does only line breaks yet I'll leave it here
	FText formatFText(const FText &textToFormat);


	// indicates what element number of dialogTexts is displayed
	float dialogState = 1;
	FText dialogText;
	

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LittleBigTown.h"

#include "UIBuildingButton.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class LITTLEBIGTOWN_API UUIBuildingButton : public UUserWidget
{
	GENERATED_BODY()

public :

	virtual void NativePreConstruct() override;

	// Set Style and variable
	void SetButtonClicked(bool IsClicked);

	// Set Style and variable
	// If not in ThematicWidget, Set visibility of Button on Collapsed
	void SetButtonDisabled(bool IsDisabled, bool ThematicWidget);

	bool GetButtonClicked();

	UFUNCTION(BlueprintCallable)
		const FName& GetButtonText() { return Name; }

protected :
	UPROPERTY(EditAnywhere, Category = "Style")
		struct FButtonStyle BasicStyle;

	UPROPERTY(EditDefaultsOnly, Category = "Style")
		struct FButtonStyle OnClickedStyle;

	UPROPERTY(EditDefaultsOnly, Category = "Style")
		struct FButtonStyle OnDisabledStyle;

	UPROPERTY(EditAnywhere, Category = "Style")
		FName Name;

	UPROPERTY(BlueprintReadOnly)
		bool ButtonClicked{ false };

	UPROPERTY(BlueprintReadOnly)
		bool ButtonDisabled { false };

	UPROPERTY(meta = (BindWidget))
		class UButton* Button;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* TextBlock;

};

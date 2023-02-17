
#pragma once

#include "CoreMinimal.h"
#include "LittleBigTown/UserInterface/Templates/BGLayerWidget.h"
#include "BGSComboBoxWidget.generated.h"

UCLASS(Abstract)
class LITTLEBIGTOWN_API UBGSComboBoxWidget : public UBGLayerWidget
{
	GENERATED_BODY()

public : 
	
	UFUNCTION()
		virtual void ComboSelectionChanged(class UBGComboBoxString* ComboBox, const FString & Option, int32 Index);

protected : 

	UPROPERTY(EditAnywhere)
		FText DefaultOption;
	
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetBuildingValidation.h"
#include "Components/TextBlock.h"


void UWidgetBuildingValidation::DisplayValidationWidget(const FName& Name)
{
	FString S{ Name.ToString() };
	S.Append(" ?");
	BuildingName->SetText(FText::FromString(S));

	this->SetVisibility(ESlateVisibility::Visible);
}

const FText UWidgetBuildingValidation::GetBuildingName() const
{
	FString S { BuildingName->GetText().ToString() };
	S = S.Mid(0, S.Len() - 2);

	return FText::FromString(S);
}

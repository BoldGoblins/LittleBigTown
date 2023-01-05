// Fill out your copyright notice in the Description page of Project Settings.

#include "LittleBigTown.h"

#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, LittleBigTown, "LittleBigTown" );

void MyPersonalLibrary::AddOptionsToComboBoxString(UComboBoxString* ComboBox, const TArray<FString>& Options)
{
#ifdef DEBUG_ONLY

	checkf(ComboBox && Options.Num() != 0, TEXT("Error in MyPersonalLibrary::AddOptionsToComboBoxString : ComboBox not valid or Options array empty."));

#endif

	if (!ComboBox || Options.Num() == 0)
		return;

	ComboBox->ClearOptions();

	for (const auto S : Options)
		ComboBox->AddOption(S);

	ComboBox->RefreshOptions();
	ComboBox->SetSelectedIndex(0);
}

FString Timer::GetStringTime()
{
	FString S;
	S.AppendInt(m_Hour);
	S.Append(" : ");
	S.AppendInt(m_Min);
	S.Append(" : ");
	S.AppendInt(m_Sec);
	S.Append(" : ");
	S.AppendInt(m_MSec);

	return S;
}

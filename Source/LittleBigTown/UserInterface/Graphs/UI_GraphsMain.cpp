// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleBigTown/UserInterface/Graphs/UI_GraphsMain.h"
#include "LittleBigTown/UserInterface/Graphs/UI_GraphDemand.h"
#include "LittleBigTown/GameSystem/MainPlayerController.h"
#include "LittleBigTown/Core/Debugger.h"

#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UUI_GraphsMain::NativeConstruct()
{
	Super::NativeConstruct();

	const auto PC { Cast <AMainPlayerController> (UGameplayStatics::GetPlayerController(GetWorld(), 0)) };
	PC->SetGraphWidget(this);

	GraphDemand = Cast <UUI_GraphDemand> (WidgetSwitcher->GetChildAt(0));
}

void UUI_GraphsMain::DisplayGraph(int32 Index)
{

#ifdef DEBUG_ONLY

	checkf(Index == 0, TEXT("Error in UUI_GraphsMain::DisplayGraph, Index != 0."));

#endif

	GraphDemand->Display();
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UUI_GraphsMain::HideWidget()
{
	// Call all the hide functions into this one
	GraphDemand->Hide();
	SetVisibility(ESlateVisibility::Collapsed);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "LittleBigTown/UserInterface/Main/UI_Demand.h"
#include "Components/ProgressBar.h"

void UUI_Demand::UpdateDemand(float PoorDemand, float MiddleDemand, float RichDemand)
{

	PB_PoorNeg->SetVisibility(ESlateVisibility::Hidden);
	PB_MiddleNeg->SetVisibility(ESlateVisibility::Hidden);
	PB_RichNeg->SetVisibility(ESlateVisibility::Hidden);
	PB_PoorPos->SetVisibility(ESlateVisibility::Hidden);
	PB_MiddlePos->SetVisibility(ESlateVisibility::Hidden);
	PB_RichPos->SetVisibility(ESlateVisibility::Hidden);

	if (PoorDemand < 0)
		PB_PoorNeg->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	else
		PB_PoorPos->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if(MiddleDemand < 0)
		PB_MiddleNeg->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	else
		PB_MiddlePos->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	if(RichDemand <0)
		PB_RichNeg->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	else
		PB_RichPos->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	PoorDemand < 0 ? PB_PoorNeg->SetPercent(PoorDemand * -1) : PB_PoorPos->SetPercent(PoorDemand);
	MiddleDemand < 0 ? PB_MiddleNeg->SetPercent(MiddleDemand * -1) : PB_MiddlePos->SetPercent(MiddleDemand);
	RichDemand < 0 ? PB_RichNeg->SetPercent(RichDemand * -1) : PB_RichPos->SetPercent(RichDemand);
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"



#include "BGMainPlayerController.generated.h"


UCLASS()
class LITTLEBIGTOWN_API ABGMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public : 


	// --------------------------------------		GENERAL (AND HERITED) FUNCTIONS		--------------------------------------


	ABGMainPlayerController();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

	void SetViewportSize(FViewport* ViewPort, uint32 Val);


	// --------------------------------------		PAWN CONTROL FUNCTIONS		  --------------------------------------

	int32 GetZoomNotchesNum() const { return _MaxZoomNotches; }

	void ScrollForward(float Axis);

	void ScrollRight(float Axis);

	void PitchRotation(float Axis);

	void YawRotation(float Axis);

	void KeyboardPitchRotation(float Axis);

	void KeyboardYawRotation(float Axis);

	void EnableMouseCaptureRotation();

	void DisableMouseCaptureRotation();

	void Zoom(float Axis);

private :

	void EdgeScrolling();


	// --------------------------------------		WIDGETS FUNCTIONS		  --------------------------------------


public :

	// Plusieurs fonctions car besoin de renvoyer direct le bon type pour éviter d'avoir à recaster derrière (Getters souvent appellés)
	// Idem pour les Setters, permet d'éviter d'envoyer le mauvais type (effectue un Type-Check via l'argument)

	UFUNCTION(BlueprintCallable)
		class UUI_ConstructionMain* GetConstructionWidget();

	UFUNCTION(BlueprintCallable)
		class UUI_Main* GetMainWidget();

	UFUNCTION(BlueprintCallable)
		class UUI_GraphsMain* GetGraphsWidget() const;

	class UUI_BuildingInfos* GetBuildingInfos() const;

	void SetMainWidget(class UUI_Main* Widget);

	void SetBuildingInfosWidget(class UUI_BuildingInfos* Widget);

	void SetGraphWidget(class UUI_GraphsMain* Widget);

	void SetConstructionWidget(class UUI_ConstructionMain* Widget);


	// --------------------------------------		WIDGETS PARAMETERS		  --------------------------------------

protected:


	UPROPERTY(BlueprintReadOnly)
		class UUI_ConstructionMain* _ConstructionWidget{ nullptr };

	UPROPERTY(BlueprintReadOnly)
		class UUI_Main* _MainWidget{ nullptr };

	UPROPERTY(BlueprintReadOnly)
		class UUI_BuildingInfos* _BuildingInfosWidget{ nullptr };

	UPROPERTY(BlueprintReadOnly)
		class UUI_GraphsMain* _GraphsWidget{ nullptr };


	// --------------------------------------		VIEWPORT PARAMETERS		  --------------------------------------


	UPROPERTY()
		FIntPoint _ViewportSize {};

	// Set the input mode to game in rotation (to avoid mouse capture issues)
	FInputModeGameOnly InputModeGameOnly{};

	// Set the input mode to game and ui otherwise (maybe just UI ?)
	FInputModeGameAndUI InputModeGameAndUI{};


	// --------------------------------------		PAWN CONTROL PARAMETERS		  --------------------------------------


	UPROPERTY(BlueprintReadOnly)
		TWeakObjectPtr <class ABGPlayerPawn> _PlayerPawn;

	// Correction appliquée au Vecteur de Transform pour le déplacement en fonction du cran de Zoom actuel
	// Réduire valeur = plus forte diminution de la vitesse lorsque cran de Zoom maximal (ZoomNotch tends vers 0)
	UPROPERTY(EditDefaultsOnly)
		float _MinScrollCorrection {0.5f};

	float _ScrollCorrection { 1.0f };

	// Variable d'ajustement pour augmenter vitesse par Frame du Edge Scrolling
	// Plus grande valeur = plus grande vitesse
	UPROPERTY(EditDefaultsOnly)
		int32 _EdgeScrollingThreshold { 100 };

	// Nombre total de crans de Zoom possibles, augmenter valeur = même longueur de Zoom mais divisée en plus de segments
	UPROPERTY(EditDefaultsOnly)
		int32 _MaxZoomNotches { 30 };

	int32 _CurrentZoomNotch {};

	UPROPERTY()
		bool bEdgeScrollDisabled { false };

};

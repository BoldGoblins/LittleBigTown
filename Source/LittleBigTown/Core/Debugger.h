// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#define DEBUG_ONLY

// Passer FString::Printf par le param�tre String si affichage plus complexe � faire (valeurs de variables, ...)
#define PrintString(String) GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, String)

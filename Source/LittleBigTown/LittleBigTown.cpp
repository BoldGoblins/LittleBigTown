// Fill out your copyright notice in the Description page of Project Settings.

#include "LittleBigTown.h"

#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, LittleBigTown, "LittleBigTown" );

FString DEBUGGER::AssertDebugLogger(const FString& ClassName, const FString& FunctionName, const FString& Message)
{
    return "Error in " + ClassName + "::" + FunctionName + " : " + Message + ".";
}

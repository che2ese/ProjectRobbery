// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectRobberyGameMode.h"
#include "ProjectRobberyPlayerController.h"
#include "ProjectRobberyCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectRobberyGameMode::AProjectRobberyGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AProjectRobberyPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
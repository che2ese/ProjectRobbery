// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectRobberyGameMode.h"

#include "ProjectRobberyCharacter.h"
#include "ProjectRobberyPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TextBlock.h"
#include "ItemUI.h"

AProjectRobberyGameMode::AProjectRobberyGameMode()
{
    // use our custom PlayerController class
    PlayerControllerClass = AProjectRobberyPlayerController::StaticClass();

    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(
        TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
    if (PlayerPawnBPClass.Class != nullptr)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
}

void AProjectRobberyGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (ItemWidget != nullptr)
    {
        ItemUI = CreateWidget<UItemUI>(GetWorld(), ItemWidget);
        if (ItemUI != nullptr)
            ItemUI->AddToViewport();
    }
    PrintItems();
}

void AProjectRobberyGameMode::PrintItems()
{
    if (ItemUI != nullptr)
    {
        ItemUI->countCoat->SetText(FText::AsNumber(coats));
        ItemUI->countNoise->SetText(FText::AsNumber(noises));
        ItemUI->countCamera->SetText(FText::AsNumber(cameras));
    }
}

void AProjectRobberyGameMode::AddItems(EItemType Item)
{
    if (Item == EItemType::Item_Noise)
        noises++;
    if (Item == EItemType::Item_Coat)
        coats++;
    if (Item == EItemType::Item_Camera)
        cameras++;
    
    PrintItems();
}

void AProjectRobberyGameMode::UseItems(EItemType Item)
{
    if (Item == EItemType::Item_Noise)
        noises--;
    if (Item == EItemType::Item_Coat)
        coats--;
    if (Item == EItemType::Item_Camera)
        cameras--;

    PrintItems();
}
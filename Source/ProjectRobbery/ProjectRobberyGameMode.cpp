// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectRobberyGameMode.h"

#include "ProjectRobbery.h"
#include "ProjectRobberyCharacter.h"
#include "ProjectRobberyPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TextBlock.h"
#include "ItemUI.h"
#include "MainWidget.h"
#include "LevelClearWidget.h"
#include "GameOverWidget.h"
#include "Kismet/GameplayStatics.h"

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
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AExitGoal::StaticClass(), FoundActors);
     
    if (FoundActors.Num() > 0)
    {
        AExitGoal* FoundExitGoal = Cast<AExitGoal>(FoundActors[0]);
        if (FoundExitGoal)
        {
            points = FoundExitGoal->RequiredPoint;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("FoundExitGoal is nullptr"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No AExitGoal found in the level"));
    }

    if (ItemWidget != nullptr)
    {
        ItemUI = CreateWidget<UItemUI>(GetWorld(), ItemWidget);
        if (ItemUI != nullptr)
            ItemUI->AddToViewport();
    }

    if (IsValid(mainWidget))
    {
        mainUI = CreateWidget<UMainWidget>(GetWorld(), mainWidget);
        if (IsValid(mainUI))
        {
            mainUI->AddToViewport();
        }
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
        ItemUI->countPoint->SetText(FText::AsNumber(points));
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
    if (Item == EItemType::Item_Point)
        points--;
    
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

void AProjectRobberyGameMode::ShowGameOverMenu()
{
    if (menuWidget == nullptr)
    {
        FMT_ERROR(TEXT("Menu widget is null"));
        return;
    }

    auto *menuUI = CreateWidget<UGameOverWidget>(GetWorld(), menuWidget);
    if (!IsValid(menuUI))
    {
        FMT_ERROR(TEXT("Unable to create menuUI"));
        return;
    }

    menuUI->AddToViewport();

    UGameplayStatics::SetGamePaused(GetWorld(), true);
    // GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}

void AProjectRobberyGameMode::ShowClearMenu()
{
    if (nextLevelWidget == nullptr)
    {
        FMT_ERROR(TEXT("Next level widget is null"));
        return;
    }

    auto *nextLevelUI = CreateWidget<ULevelClearWidget>(GetWorld(), nextLevelWidget);
    if (!IsValid(nextLevelUI))
    {
        FMT_ERROR(TEXT("Unable to create nextLevelUI"));
        return;
    }

    nextLevelUI->AddToViewport();

    UGameplayStatics::SetGamePaused(GetWorld(), true);
    // GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}
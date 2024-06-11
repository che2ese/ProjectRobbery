#include "LevelClearWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

EMapType ULevelClearWidget::currentMap = EMapType::Map1;

void ULevelClearWidget::NativeConstruct()
{
    Super::NativeConstruct();

    nextLevelBtn->OnClicked.AddDynamic(this, &ULevelClearWidget::MoveToNextLevel);
}

void ULevelClearWidget::MoveToNextLevel()
{
    switch (currentMap)
    {
        case EMapType::Map1:
            currentMap = EMapType::Map2;
            UGameplayStatics::OpenLevel(GetWorld(), "Level2");
            break;

        case EMapType::Map2:
            currentMap = EMapType::Map3;
            UGameplayStatics::OpenLevel(GetWorld(), "Level3");
            break;

        case EMapType::Map3:
            currentMap = EMapType::Map1;
            UGameplayStatics::OpenLevel(GetWorld(), "Level1");
            break;
    }
}

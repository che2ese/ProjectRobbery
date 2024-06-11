#include "GameOverWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UGameOverWidget::NativeConstruct()
{
    Super::NativeConstruct();

    button_Restart->OnClicked.AddDynamic(this, &UGameOverWidget::Restart);
    button_Quit->OnClicked.AddDynamic(this, &UGameOverWidget::Quit);
}

void UGameOverWidget::Restart()
{
    UGameplayStatics::OpenLevel(GetWorld(), "Level1");
}

void UGameOverWidget::Quit()
{
    UWorld *currentWorld = GetWorld();
    UKismetSystemLibrary::QuitGame(currentWorld, currentWorld->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

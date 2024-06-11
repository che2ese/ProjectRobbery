#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

class UButton;

UCLASS()
class PROJECTROBBERY_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    void NativeConstruct() override;

private:
    UFUNCTION()
    void Restart();

    UFUNCTION()
    void Quit();

private:
    UPROPERTY(VisibleAnywhere, meta = (BindWidget))
    UButton *button_Restart;

    UPROPERTY(VisibleAnywhere, meta = (BindWidget))
    UButton *button_Quit;
};

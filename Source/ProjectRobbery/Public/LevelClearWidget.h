#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelClearWidget.generated.h"

class UButton;

enum class EMapType
{
    Map1,
    Map2,
	Map3,
};

UCLASS()
class PROJECTROBBERY_API ULevelClearWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void NativeConstruct() override;

private:
    UFUNCTION()
    void MoveToNextLevel();
	
private:
	static EMapType currentMap;

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UButton* nextLevelBtn;
};

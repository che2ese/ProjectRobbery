#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

class UTextBlock;

UCLASS()
class PROJECTROBBERY_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* HealthText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* SteminaText;
    ;
};

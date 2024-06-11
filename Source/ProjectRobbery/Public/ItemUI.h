// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTROBBERY_API UItemUI : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, meta = (BindWidget))
    class UTextBlock* countCoat;
    UPROPERTY(EditAnywhere, meta = (BindWidget))
    class UTextBlock* countNoise;
    UPROPERTY(EditAnywhere, meta = (BindWidget))
    class UTextBlock* countCamera;
};

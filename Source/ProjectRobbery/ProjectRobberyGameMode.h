// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ItemBase.h"
#include "ProjectRobberyGameMode.generated.h"

UCLASS(minimalapi)
class AProjectRobberyGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AProjectRobberyGameMode();

    UPROPERTY(EditAnywhere)
    TSubclassOf<class UItemUI> ItemWidget;

    UPROPERTY(EditAnywhere)
    int32 coats = 0;
    UPROPERTY(EditAnywhere)
    int32 cameras = 0;
    UPROPERTY(EditAnywhere)
    int32 noises = 0;

    UFUNCTION()
    void AddItems(EItemType Item);
    UFUNCTION()
    void UseItems(EItemType Item);
    UPROPERTY(EditAnywhere)
    class UItemUI* ItemUI;


protected:
    virtual void BeginPlay() override;

private:

    void PrintItems();
};

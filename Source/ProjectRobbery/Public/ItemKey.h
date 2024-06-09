// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "ItemKey.generated.h"

/**
 *
 */
UCLASS()
class PROJECTROBBERY_API AItemKey : public AItemBase
{
    GENERATED_BODY()

public:
    AItemKey();
    
    virtual void OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent,
                                 AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp,
                                 int32 OtherBodyIndex,
                                 bool bFromSweep,
                                 const FHitResult& SweepResult) override;

    UPROPERTY(EditAnywhere)
    int32 keyNum = 0;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    Item_None UMETA(DisplayName = "None"),
    Item_Coat UMETA(DisplayName = "Coat"),
    Item_Camera UMETA(DisplayName = "Camera"),
    Item_Key UMETA(DisplayName = "Key"),
    Item_Noise UMETA(DisplayName = "Noise"), 
    Item_Point UMETA(DisplayName = "Point")
};

UCLASS()
class PROJECTROBBERY_API AItemBase : public AActor
{
    GENERATED_BODY()

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Sets default values for this actor's properties
    AItemBase();

    UPROPERTY(EditAnywhere)
    class UBoxComponent* boxComp;

    UPROPERTY(EditAnywhere)
    class UStaticMeshComponent* meshComp;

    UPROPERTY(EditAnywhere)
    EItemType Item;

    UFUNCTION()
    virtual void OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent,
                         AActor* OtherActor,
                         UPrimitiveComponent* OtherComp,
                         int32 OtherBodyIndex,
                         bool bFromSweep,
                         const FHitResult& SweepResult);

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
};

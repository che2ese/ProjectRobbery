// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TimelineComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class PROJECTROBBERY_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

    UPROPERTY(EditAnywhere)
    class UBoxComponent* boxComp;
	UPROPERTY(EditAnywhere)
    class UStaticMeshComponent* doorComp;

    UPROPERTY(EditAnywhere)
    class UBoxComponent* doorCollisionComp;

    UPROPERTY(EditAnywhere)
    bool bLocked = false;

    UPROPERTY(EditAnywhere)
    int32 doorNum = 0;

    UFUNCTION()
    void OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent,
                         AActor* OtherActor,
                         UPrimitiveComponent* OtherComp,
                         int32 OtherBodyIndex,
                         bool bFromSweep,
                         const FHitResult& SweepResult);
    UFUNCTION()
    void OffPlayerOverlap(UPrimitiveComponent* OverlappedComponent,
                         AActor* OtherActor,
                         UPrimitiveComponent* OtherComp,
                         int32 OtherBodyIndex);

    void OpenDoor();
    void CloseDoor();
    bool isOpenDoor = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

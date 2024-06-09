// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera.generated.h"

UCLASS()
class PROJECTROBBERY_API ACamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
    ACamera();
    UPROPERTY(EditAnywhere)
    class UCapsuleComponent* capsuleComp;

    UPROPERTY(EditAnywhere)
    class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere)
    bool enable = true;

	UFUNCTION()
    void OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent,
                         AActor* OtherActor,
                         UPrimitiveComponent* OtherComp,
                         int32 OtherBodyIndex,
                         bool bFromSweep,
                         const FHitResult& SweepResult);

    UPROPERTY(EditAnywhere)
    float countdown = 10;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

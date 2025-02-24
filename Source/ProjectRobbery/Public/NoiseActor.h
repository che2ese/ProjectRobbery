// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NoiseActor.generated.h"

UCLASS()
class PROJECTROBBERY_API ANoiseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANoiseActor();
    UPROPERTY(EditAnywhere)
    class UBoxComponent* boxComp;

    UPROPERTY(EditAnywhere)
    class UStaticMeshComponent* meshComp;

    UPROPERTY(EditAnywhere)
    float countdown = 10;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

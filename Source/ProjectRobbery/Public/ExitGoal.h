// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExitGoal.generated.h"

UCLASS()
class PROJECTROBBERY_API AExitGoal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExitGoal();
    UPROPERTY(EditAnywhere)
    class USphereComponent* sphereComp;

    UPROPERTY(EditAnywhere)
    class UStaticMeshComponent* meshComp;

    UFUNCTION()
    virtual void OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent,
                                 AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp,
                                 int32 OtherBodyIndex,
                                 bool bFromSweep,
                                 const FHitResult& SweepResult);
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GamePlay")
    int RequiredPoint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
    bool bMissionCompleted;    // 미션 완료 상태
};

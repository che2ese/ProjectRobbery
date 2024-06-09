#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "EnemyCharacter.generated.h"

UCLASS()
class PROJECTROBBERY_API AEnemyCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AEnemyCharacter();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    TArray<AActor*> Waypoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float WaypointAcceptanceRadius = 100.f;

    int32 CurrentWaypointIndex = -1;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};

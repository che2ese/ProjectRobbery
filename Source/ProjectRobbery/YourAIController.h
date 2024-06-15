#pragma once

#include "AIController.h"
#include "CoreMinimal.h"
#include "EnemyCharacter.h"
#include "Camera.h"
#include "YourAIController.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;
class AEnemyCharacter;


UCLASS(config = Game)
class PROJECTROBBERY_API AYourAIController : public AAIController
{
    GENERATED_BODY()

public:
    AYourAIController();

    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;
    virtual void Tick(float DeltaSeconds) override;
    virtual FRotator GetControlRotation() const override;



    UFUNCTION()
    virtual void OnPerceptionUpdated(const TArray<AActor*>& DetectedPawns);

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
    float AISightRadius = 450.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
    float AISightAge = 5.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
    float AILoseSightRadius = AISightRadius + 150.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
    float AIFieldOfView = 85.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
    class UAISenseConfig_Sight* SightConfig;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
    class UAISenseConfig_Hearing* HearingConfig;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
    bool bIsPlayerDetected = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
    bool bIsPlayerHearDetected = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
    float DistanceToPlayer = 0.f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
    float AIHearingRange = 500.f;    // AI가 소리를 감지하는 범위

    UPROPERTY(EditAnywhere)
    bool cameraActive = false;
    UPROPERTY(EditAnywhere)
    bool coatActive = false;
    UPROPERTY(EditAnywhere)
    float atkCount = 10;
    UPROPERTY(EditAnywhere)
    float coatCount = 10;

    UPROPERTY(EditAnywhere)
    class ACamera* camera;

private:
    UBlackboardComponent* BlackboardComp;
    UBehaviorTreeComponent* BehaviorComp;

    UPROPERTY()
    AEnemyCharacter* EnemyCharacter;

    void ReturnToWaypoints();
};
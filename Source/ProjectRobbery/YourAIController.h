#pragma once

#include "AIController.h"
#include "CoreMinimal.h"
#include "EnemyCharacter.h"

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
    void OnPawnDetected(const TArray<AActor*>& DetectedPawns);

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
    bool bIsPlayerDetected = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
    float DistanceToPlayer = 0.f;

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
    AEnemyCharacter* EnemyCharacter;    // AEnemyCharacter 클래스의 포인터 선언

    void ReturnToWaypoints();    // 웨이포인트로 돌아가는 함수 선언
};
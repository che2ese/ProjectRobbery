#include "YourAIController.h"

#include "MyTestCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

AYourAIController::AYourAIController()
{
    PrimaryActorTick.bCanEverTick = true;

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

    SightConfig->SightRadius = AISightRadius;
    SightConfig->LoseSightRadius = AILoseSightRadius;
    SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
    SightConfig->SetMaxAge(AISightAge);

    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

    GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
    GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AYourAIController::OnPawnDetected);
    GetPerceptionComponent()->ConfigureSense(*SightConfig);

    // EnemyCharacter ������ �ʱ�ȭ
    EnemyCharacter = nullptr;
}

void AYourAIController::BeginPlay()
{
    Super::BeginPlay();

    if (GetPerceptionComponent() != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("All Systems Set!"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No Component"));
    }
}

void AYourAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // ĳ���Ͱ� EnemyCharacter Ŭ������ �ν��Ͻ����� Ȯ��
    AEnemyCharacter* TestEnemyCharacter = Cast<AEnemyCharacter>(InPawn);
    if (TestEnemyCharacter != nullptr)
    {
        // EnemyCharacter ������ ����
        EnemyCharacter = TestEnemyCharacter;
    }
}

void AYourAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (bIsPlayerDetected)
    {
        AMyTestCharacter* Player = Cast<AMyTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
        if (Player)
        {
            DistanceToPlayer = FVector::Dist(Player->GetActorLocation(), EnemyCharacter->GetActorLocation());
            if (DistanceToPlayer > AILoseSightRadius)
            {
                bIsPlayerDetected = false;
                UE_LOG(LogTemp, Warning, TEXT("Player Lost!"));
                ReturnToWaypoints();
            }
            else
            {
                MoveToActor(Player, 2.0f);
            }
        }
    }
    else
    {
        if (EnemyCharacter != nullptr)
        {
            ReturnToWaypoints();
        }
    }
}


FRotator AYourAIController::GetControlRotation() const
{
    if (GetPawn() == nullptr)
    {
        return FRotator(0.f, 0.f, 0.f);
    }
    return FRotator(0.f, GetPawn()->GetActorRotation().Yaw, 0.f);
}

void AYourAIController::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
    bool bPreviouslyDetected = bIsPlayerDetected;    // ������ �÷��̾ �����߾����� Ȯ��

    bIsPlayerDetected = false;    // �ʱ�ȭ

    for (AActor* DetectedPawn : DetectedPawns)
    {
        if (DetectedPawn->IsA<AMyTestCharacter>())
        {
            bIsPlayerDetected = true;
            break;    // �÷��̾ ���������Ƿ� ���� ����
        }
    }

    // �÷��̾ �������� ���� ���
    if (!bIsPlayerDetected && bPreviouslyDetected)
    {
        UE_LOG(LogTemp, Warning, TEXT("Player Lost!"));
        if (EnemyCharacter != nullptr)
        {
            ReturnToWaypoints();
        }
    }
}

void AYourAIController::ReturnToWaypoints()
{
    if (EnemyCharacter->Waypoints.Num() > 0)
    {
        if (EnemyCharacter->CurrentWaypointIndex == -1)
        {
            EnemyCharacter->CurrentWaypointIndex = 0;
        }

        FVector TargetLocation = EnemyCharacter->Waypoints[EnemyCharacter->CurrentWaypointIndex]->GetActorLocation();
        MoveToLocation(TargetLocation, 5.0f);

        FVector CurrentLocation = EnemyCharacter->GetActorLocation();
        float DistanceToWaypoint = FVector::Distance(CurrentLocation, TargetLocation);
        if (DistanceToWaypoint < EnemyCharacter->WaypointAcceptanceRadius)
        {
            // ���� ��������Ʈ�� �̵�
            EnemyCharacter->CurrentWaypointIndex =
                (EnemyCharacter->CurrentWaypointIndex + 1) % EnemyCharacter->Waypoints.Num();
        }
    }
}
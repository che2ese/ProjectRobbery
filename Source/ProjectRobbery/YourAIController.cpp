#include "YourAIController.h"

#include "Camera.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyTestCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

AYourAIController::AYourAIController()
{
    PrimaryActorTick.bCanEverTick = true;

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
    SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

    SightConfig->SightRadius = AISightRadius;
    SightConfig->LoseSightRadius = AILoseSightRadius;
    SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
    SightConfig->SetMaxAge(AISightAge);
    HearingConfig->HearingRange = AIHearingRange;

    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

    HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
    HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
    HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

    GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
    GetPerceptionComponent()->ConfigureSense(*SightConfig);
    GetPerceptionComponent()->ConfigureSense(*HearingConfig);

    // 이벤트를 하나의 함수로 등록
    GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AYourAIController::OnPerceptionUpdated);

    // EnemyCharacter 포인터 초기화
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

    // 캐릭터가 EnemyCharacter 클래스의 인스턴스인지 확인
    AEnemyCharacter* TestEnemyCharacter = Cast<AEnemyCharacter>(InPawn);
    if (TestEnemyCharacter != nullptr)
    {
        // EnemyCharacter 포인터 설정
        EnemyCharacter = TestEnemyCharacter;
    }
}

void AYourAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    atkCount -= 1 * DeltaSeconds;
    if (coatActive)
    {
        coatCount -= 1 * DeltaSeconds;
        if (coatCount <= 0.5)
        {
            coatCount = 10;
            coatActive = false;
        }
    }
    if (cameraActive)
    {
        MoveToActor(camera, 2.0f);
        float distanceToCamera = GetPawn()->GetDistanceTo(camera);
        if (distanceToCamera <= 100 || (bIsPlayerDetected && bIsPlayerHearDetected && !coatActive))
        {
            cameraActive = false;
            UE_LOG(LogTemp, Warning, TEXT("Shut down camera"));
        }
    }
    if (bIsPlayerDetected && !coatActive)
    {
        AMyTestCharacter* Player = Cast<AMyTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
        if (Player)
        {
            DistanceToPlayer = FVector::Dist(Player->GetActorLocation(), EnemyCharacter->GetActorLocation());
            if (DistanceToPlayer > AILoseSightRadius)
            {
                bIsPlayerDetected = false;
                UE_LOG(LogTemp, Warning, TEXT("Player Lost!"));
                EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = 150.f;    // 속도 원래대로
                ReturnToWaypoints();
            }
            else
            {
                if (DistanceToPlayer < 30.f)
                {
                    Player->ReduceHealth(2 * DeltaSeconds);
                    UE_LOG(LogTemp, Warning, TEXT("Damage"));
                }
                UE_LOG(LogTemp, Log, TEXT("SEE"));
                EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = 300.f;
                MoveToActor(Player, 2.0f);
            }
        }
    }
    else if (bIsPlayerHearDetected && !coatActive)
    {
        AMyTestCharacter* Player = Cast<AMyTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
        if (Player)
        {
            DistanceToPlayer = FVector::Dist(Player->GetActorLocation(), EnemyCharacter->GetActorLocation());
            if (DistanceToPlayer > AIHearingRange)
            {
                bIsPlayerHearDetected = false;
                UE_LOG(LogTemp, Warning, TEXT("Player Lost!"));
                EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = 150.f;    // 속도 원래대로
                ReturnToWaypoints();
            }
            else
            {
                UE_LOG(LogTemp, Log, TEXT("HEAR"));
                EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = 300.f;
                MoveToActor(Player, 2.0f);
            }
        }
    }
    else
    {
        if (EnemyCharacter != nullptr)
        {
            EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = 150.f;
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

void AYourAIController::OnPerceptionUpdated(const TArray<AActor*>& DetectedPawns)
{
    bool bPreviouslyDetected = bIsPlayerDetected;    // 이전에 플레이어를 감지했었는지 확인

    bIsPlayerDetected = false;
    bIsPlayerHearDetected = false;

    for (AActor* DetectedPawn : DetectedPawns)
    {
        if (DetectedPawn->IsA<AMyTestCharacter>())
        {
            bIsPlayerDetected = true;
            bIsPlayerHearDetected = true;
            break;
        }
    }

    // 플레이어를 감지하지 못한 경우
    if (!bIsPlayerDetected && !bIsPlayerHearDetected && bPreviouslyDetected)
    {
        UE_LOG(LogTemp, Warning, TEXT("Player Lost!"));
        if (EnemyCharacter != nullptr)
        {
            ReturnToWaypoints();
        }
    }
    else
    {
        if (bIsPlayerDetected)
        {
            UE_LOG(LogTemp, Log, TEXT("Player Seen!"));
        }
        if (bIsPlayerHearDetected)
        {
            UE_LOG(LogTemp, Log, TEXT("Player Heard!"));
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
            // 다음 웨이포인트로 이동
            EnemyCharacter->CurrentWaypointIndex =
                (EnemyCharacter->CurrentWaypointIndex + 1) % EnemyCharacter->Waypoints.Num();
        }
    }
}

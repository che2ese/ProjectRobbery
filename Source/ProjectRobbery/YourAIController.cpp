#include "YourAIController.h"

#include "Camera.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyTestCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h" // 경로 탐색을 위한 헤더
#include "ProjectRobbery.h"
#include "NoiseActor.h"
#include "EngineUtils.h" // TActorIterator를 사용하기 위한 헤더 파일입니다.


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

    GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AYourAIController::OnPerceptionUpdated);

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

    AEnemyCharacter* TestEnemyCharacter = Cast<AEnemyCharacter>(InPawn);
    if (TestEnemyCharacter != nullptr)
    {
        EnemyCharacter = TestEnemyCharacter;
    }
}

void AYourAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    AMyTestCharacter* Player = Cast<AMyTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
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
    // 카메라가 활성화된 상태일 때
    if (cameraActive)
    {
        // 카메라 위치 가져오기
        FVector CameraLocation = camera->GetActorLocation();

        // 현재 AI의 위치
        FVector AIPosition = GetPawn()->GetActorLocation();

        // AI가 유지할 거리 범위
        float MinDistanceToCamera = 100.0f;     // 최소 거리
        float MaxDistanceToCamera = 1000.0f;    // 최대 거리

        // AI와 카메라 사이의 거리 계산
        float DistanceToCamera = FVector::Dist(CameraLocation, AIPosition);

        // AI가 이동할 방향 벡터 계산
        FVector DirectionToCamera = CameraLocation - AIPosition;
        DirectionToCamera.Z = 0.0f;    // 수직 방향 제거
        DirectionToCamera.Normalize();

        // 목표 위치 설정: 카메라 방향으로 이동
        FVector TargetLocation = CameraLocation - DirectionToCamera * MinDistanceToCamera;

        // 이동 명령 수행
        MoveToLocation(TargetLocation, 2.0f);    // 4.0f는 이동 속도

        // AI가 카메라와의 거리 범위를 넘어선 경우 또는 플레이어를 감지한 경우
        if ((DistanceToCamera <= 200 || DistanceToCamera >= MaxDistanceToCamera) ||
            (bIsPlayerDetected && bIsPlayerHearDetected && !coatActive))
        {
            // 카메라 근처에 왔을 때의 처리
            cameraActive = false;
            UE_LOG(LogTemp, Warning, TEXT("Shut down camera"));

            // 다시 waypoint로 돌아가는 동작 추가
            if (EnemyCharacter != nullptr)
            {
                EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = 150.f;
                ReturnToWaypoints();
            }
        }
    }
    // noiseActive가 true인 경우에만 noiseActor를 쫓아갑니다.
    else if (Player && Player->noiseActive)
    {
        ANoiseActor* NoiseActor = nullptr;
        for (TActorIterator<ANoiseActor> It(GetWorld()); It; ++It)
        {
            NoiseActor = *It;
            if (NoiseActor != nullptr)
            {
                // NoiseActor가 있다면 해당 위치로 이동합니다.
                MoveToActor(NoiseActor, 2.0f);
                break;
            }
        }

        // NoiseActor가 없는 경우 또는 NoiseActor에 도착한 경우
        if (NoiseActor == nullptr ||
            FVector::Dist(NoiseActor->GetActorLocation(), GetPawn()->GetActorLocation()) < 100.0f)
        {
            Player->noiseActive = false;
            // 추가적인 동작을 넣으시면 됩니다 (예: 웨이포인트로 돌아가기)
            ReturnToWaypoints();
        }
    }
    // 플레이어를 감지하거나 들을 때
    else if (bIsPlayerDetected && !coatActive)
    {
        if (Player)
        {
            DistanceToPlayer = FVector::Dist(Player->GetActorLocation(), EnemyCharacter->GetActorLocation());
            if (DistanceToPlayer > AILoseSightRadius)
            {
                bIsPlayerDetected = false;
                UE_LOG(LogTemp, Warning, TEXT("Player Lost!"));
                EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = 150.f;
                ReturnToWaypoints();
            }
            else
            {
                if (DistanceToPlayer < 150.f)
                {
                    Player->ReduceHealth(1 * DeltaSeconds);
                }
                EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = 300.f;
                MoveToActor(Player, 2.0f);
            }
        }
    }
    else if (bIsPlayerHearDetected && !coatActive)
    {
        if (Player)
        {
            DistanceToPlayer = FVector::Dist(Player->GetActorLocation(), EnemyCharacter->GetActorLocation());
            if (DistanceToPlayer > AIHearingRange)
            {
                bIsPlayerHearDetected = false;
                UE_LOG(LogTemp, Warning, TEXT("Player Lost!"));
                EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = 150.f;
                ReturnToWaypoints();
            }
            else
            {
                EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = 300.f;
                MoveToActor(Player, 2.0f);
            }
        }
    }
    // 기본적인 상황에서는 waypoint로 이동
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
    bool bPreviouslyDetected = bIsPlayerDetected; 

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

            EnemyCharacter->CurrentWaypointIndex =
                (EnemyCharacter->CurrentWaypointIndex + 1) % EnemyCharacter->Waypoints.Num();
        }
    }
}

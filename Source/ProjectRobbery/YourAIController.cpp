#include "YourAIController.h"

#include "Camera.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyTestCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "ProjectRobbery.h"

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

    // �̺�Ʈ�� �ϳ��� �Լ��� ���
    GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AYourAIController::OnPerceptionUpdated);

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
                EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = 150.f;    // �ӵ� �������
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
        AMyTestCharacter* Player = Cast<AMyTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
        if (Player)
        {
            DistanceToPlayer = FVector::Dist(Player->GetActorLocation(), EnemyCharacter->GetActorLocation());
            if (DistanceToPlayer > AIHearingRange)
            {
                bIsPlayerHearDetected = false;
                UE_LOG(LogTemp, Warning, TEXT("Player Lost!"));
                EnemyCharacter->GetCharacterMovement()->MaxWalkSpeed = 150.f;    // �ӵ� �������
                ReturnToWaypoints();
            }
            else
            {
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
    bool bPreviouslyDetected = bIsPlayerDetected;    // ������ �÷��̾ �����߾����� Ȯ��

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

    // �÷��̾ �������� ���� ���
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
            // ���� ��������Ʈ�� �̵�
            EnemyCharacter->CurrentWaypointIndex =
                (EnemyCharacter->CurrentWaypointIndex + 1) % EnemyCharacter->Waypoints.Num();
        }
    }
}

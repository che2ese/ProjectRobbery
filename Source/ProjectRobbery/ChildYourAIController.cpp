#include "ChildYourAIController.h"

#include "Kismet/GameplayStatics.h"
#include "MyTestCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

void AChildYourAIController::OnPerceptionUpdated(const TArray<AActor*>& DetectedPawns)
{
    Super::OnPerceptionUpdated(DetectedPawns);

    bool bPlayerSeen = false;
    bool bPlayerHeard = false;

    for (AActor* DetectedPawn : DetectedPawns)
    {
        if (DetectedPawn->IsA<AMyTestCharacter>())
        {
            bPlayerSeen = true;
            bPlayerHeard = true;
            break;
        }
    }

    UpdateAIPerceptionRanges(bPlayerSeen, bPlayerHeard);
}

void AChildYourAIController::UpdateAIPerceptionRanges(bool bPlayerSeen, bool bPlayerHeard)
{
    bool bPlayerDetected = bPlayerSeen || bPlayerHeard;

    // 플레이어가 이미 감지된 경우에만 인식 범위를 한 번만 커지게 설정
    static bool bAlreadyIncreasedRange = false;
    if (bPlayerDetected && !bAlreadyIncreasedRange)
    {
        IncreasePerceptionRanges();
        bAlreadyIncreasedRange = true;    // 인식 범위가 이미 한 번 커진 상태로 설정
    }
    else if (!bPlayerDetected && bAlreadyIncreasedRange)
    {
        DecreasePerceptionRanges();
        bAlreadyIncreasedRange = false;    // 인식 범위가 다시 줄어든 상태로 설정
    }
}

void AChildYourAIController::IncreasePerceptionRanges()
{
    if (GetWorld())
    {
        AMyTestCharacter* MyCharacter = Cast<AMyTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
        if (MyCharacter && MyCharacter->DogAudioComponent)
        {
            MyCharacter->DogAudioComponent->Play();
        }
    }
    TArray<AActor*> AIControllers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AYourAIController::StaticClass(), AIControllers);

    for (AActor* AIControllerActor : AIControllers)
    {
        AYourAIController* AIController = Cast<AYourAIController>(AIControllerActor);
        if (AIController && AIController->GetPerceptionComponent())
        {
            float SightMultiplier = 2.f;
            float HearingMultiplier = 2.f;

            AIController->SightConfig->SightRadius *= SightMultiplier;
            AIController->SightConfig->LoseSightRadius *= SightMultiplier;
            AIController->HearingConfig->HearingRange *= HearingMultiplier;

            // Re-configure senses to apply changes
            AIController->GetPerceptionComponent()->ConfigureSense(*AIController->SightConfig);
            AIController->GetPerceptionComponent()->ConfigureSense(*AIController->HearingConfig);
        }
    }
}

void AChildYourAIController::DecreasePerceptionRanges()
{
    if (GetWorld())
    {
        AMyTestCharacter* MyCharacter = Cast<AMyTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
        if (MyCharacter && MyCharacter->DogAudioComponent && MyCharacter->DogAudioComponent->IsPlaying())
        {
            MyCharacter->DogAudioComponent->Stop();
        }
    }

    TArray<AActor*> AIControllers;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AYourAIController::StaticClass(), AIControllers);

    for (AActor* AIControllerActor : AIControllers)
    {
        AYourAIController* AIController = Cast<AYourAIController>(AIControllerActor);
        if (AIController && AIController->GetPerceptionComponent())
        {
            float SightMultiplier = 0.5f;      // 감지 범위를 줄이는 배율
            float HearingMultiplier = 0.5f;    // 청각 범위를 줄이는 배율

            AIController->SightConfig->SightRadius *= SightMultiplier;
            AIController->SightConfig->LoseSightRadius *= SightMultiplier;
            AIController->HearingConfig->HearingRange *= HearingMultiplier;

            // Re-configure senses to apply changes
            AIController->GetPerceptionComponent()->ConfigureSense(*AIController->SightConfig);
            AIController->GetPerceptionComponent()->ConfigureSense(*AIController->HearingConfig);
        }
    }
}

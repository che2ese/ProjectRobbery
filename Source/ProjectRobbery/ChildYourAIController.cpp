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

    // �÷��̾ �̹� ������ ��쿡�� �ν� ������ �� ���� Ŀ���� ����
    static bool bAlreadyIncreasedRange = false;
    if (bPlayerDetected && !bAlreadyIncreasedRange)
    {
        IncreasePerceptionRanges();
        bAlreadyIncreasedRange = true;    // �ν� ������ �̹� �� �� Ŀ�� ���·� ����
    }
    else if (!bPlayerDetected && bAlreadyIncreasedRange)
    {
        DecreasePerceptionRanges();
        bAlreadyIncreasedRange = false;    // �ν� ������ �ٽ� �پ�� ���·� ����
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
            float SightMultiplier = 0.5f;      // ���� ������ ���̴� ����
            float HearingMultiplier = 0.5f;    // û�� ������ ���̴� ����

            AIController->SightConfig->SightRadius *= SightMultiplier;
            AIController->SightConfig->LoseSightRadius *= SightMultiplier;
            AIController->HearingConfig->HearingRange *= HearingMultiplier;

            // Re-configure senses to apply changes
            AIController->GetPerceptionComponent()->ConfigureSense(*AIController->SightConfig);
            AIController->GetPerceptionComponent()->ConfigureSense(*AIController->HearingConfig);
        }
    }
}

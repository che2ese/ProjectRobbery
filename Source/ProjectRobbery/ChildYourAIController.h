#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundCue.h"    // ����� ���� ��� �߰�
#include "YourAIController.h"

#include "ChildYourAIController.generated.h"

UCLASS()
class PROJECTROBBERY_API AChildYourAIController : public AYourAIController
{
    GENERATED_BODY()

public:
    virtual void OnPerceptionUpdated(const TArray<AActor*>& DetectedPawns) override;

protected:
    // ����� ������Ʈ�� �����ϰ� �ʱ�ȭ�ϴ� �Լ�
    void InitializeAudioComponent();

private:
    void UpdateAIPerceptionRanges(bool bPlayerSeen, bool bPlayerHeard);
};

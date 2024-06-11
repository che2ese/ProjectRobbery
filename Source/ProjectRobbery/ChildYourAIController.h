#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"      // GetWorld ����� ���� �߰�
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
    void IncreasePerceptionRanges();    // �ν� ������ ������Ű�� �Լ� ����
    void DecreasePerceptionRanges();    // �ν� ������ ���ҽ�Ű�� �Լ� ����

private:
    void UpdateAIPerceptionRanges(bool bPlayerSeen, bool bPlayerHeard);
};

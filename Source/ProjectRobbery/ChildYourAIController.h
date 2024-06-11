#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundCue.h"    // 오디오 관련 헤더 추가
#include "YourAIController.h"

#include "ChildYourAIController.generated.h"

UCLASS()
class PROJECTROBBERY_API AChildYourAIController : public AYourAIController
{
    GENERATED_BODY()

public:
    virtual void OnPerceptionUpdated(const TArray<AActor*>& DetectedPawns) override;

protected:
    // 오디오 컴포넌트를 생성하고 초기화하는 함수
    void InitializeAudioComponent();

private:
    void UpdateAIPerceptionRanges(bool bPlayerSeen, bool bPlayerHeard);
};

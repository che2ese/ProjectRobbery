#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"      // GetWorld 사용을 위해 추가
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
    void IncreasePerceptionRanges();    // 인식 범위를 증가시키는 함수 선언
    void DecreasePerceptionRanges();    // 인식 범위를 감소시키는 함수 선언

private:
    void UpdateAIPerceptionRanges(bool bPlayerSeen, bool bPlayerHeard);
};

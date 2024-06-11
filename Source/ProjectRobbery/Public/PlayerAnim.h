#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

UCLASS()
class PROJECTROBBERY_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

private:
	virtual void NativeUpdateAnimation(float) override;
	
public:
    UPROPERTY(Category = PlayerAnim, EditDefaultsOnly, BlueprintReadOnly)
    float speed = 0.f;
};

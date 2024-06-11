#include "PlayerAnim.h"
#include "../MyTestCharacter.h"

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    auto ownerPawn = TryGetPawnOwner();
    auto player = Cast<AMyTestCharacter>(ownerPawn);

    if (IsValid(player))
    {
        FVector velocity = player->GetVelocity();
        FVector forwardVector = player->GetActorForwardVector();

        speed = FVector::DotProduct(forwardVector, velocity);
    }
}

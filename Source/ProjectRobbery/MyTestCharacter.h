#pragma once

#include "BasicCharacter.h"
#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "EnemyCharacter.h" 
#include "MyTestCharacter.generated.h"

UCLASS()
class PROJECTROBBERY_API AMyTestCharacter : public ABasicCharacter
{
    GENERATED_BODY()

public:
    AMyTestCharacter();

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float Health;

    UFUNCTION()
    void ReduceHealth(float Amount);

    UFUNCTION()
    bool HasKey(int32 num);
    UFUNCTION()
    void UseCamera();
    UFUNCTION()
    void UseCoat();

    UPROPERTY(EditAnywhere)
    TArray<EItemType> Inventory;

    UPROPERTY(EditAnywhere)
    TArray<int32> keySet;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* FootstepSound;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
    class UAudioComponent* FootstepAudioComponent;

    FTimerHandle SprintDepletionTimerHandle;    // SprintDepletion 타이머 핸들
    void DepleteRunHealth(float DeltaTime);     // SprintDepletionRate마다 RunHealth를 감소시키는 함수
    void RecoverRunHealth(float DeltaTime);     // SprintRecoveryRate마다 RunHealth를 증가시키는 함수

private:
    void MoveForward(float Value);
    void MoveRight(float Value);

    void StartSprinting();
    void StopSprinting();
    
    void PlaySoundEvent();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;

    UPROPERTY(EditAnywhere)
    float RunHealth;    // 뛰는 동안 사용할 체력

    UPROPERTY(EditAnywhere)
    float SprintDepletionRate;

    UPROPERTY(EditAnywhere)
    float SprintRecoveryRate;

    bool bIsSprinting;
};

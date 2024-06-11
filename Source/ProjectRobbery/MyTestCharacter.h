// MyTestCharacter.h

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
    bool HasPoint();

    UFUNCTION()
    void UseCamera();

    UFUNCTION()
    void UseCoat();

    UFUNCTION()
    void UseNoise();

    UPROPERTY(EditAnywhere)
    TArray<EItemType> Inventory;

    UPROPERTY(EditAnywhere)
    TArray<int32> keySet;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* FootstepSound;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
    class UAudioComponent* FootstepAudioComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* DogSound;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
    class UAudioComponent* DogAudioComponent;    // 강아지 소리를 재생할 오디오 컴포넌트

    FTimerHandle SprintDepletionTimerHandle;    // SprintDepletion Ÿ�̸� �ڵ�
    void DepleteRunHealth(float DeltaTime);     // SprintDepletionRate���� RunHealth�� ���ҽ�Ű�� �Լ�
    void RecoverRunHealth(float DeltaTime);     // SprintRecoveryRate���� RunHealth�� ������Ű�� �Լ�

    void PlaySoundEvent();

private:
    void MoveForward(float Value);
    void MoveRight(float Value);

    // Shift Ű ���� �Լ�
    void StartSprinting();
    void StopSprinting();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;

    UPROPERTY(EditAnywhere)
    float RunHealth;    // �ٴ� ���� ����� ü��

    UPROPERTY(EditAnywhere)
    float SprintDepletionRate;

    UPROPERTY(EditAnywhere)
    float SprintRecoveryRate;

    bool bIsSprinting;
};

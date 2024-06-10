#pragma once

#include "BasicCharacter.h"
#include "CoreMinimal.h"
#include "ItemBase.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
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
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp,
                        class AActor* OtherActor,
                        class UPrimitiveComponent* OtherComp,
                        int32 OtherBodyIndex,
                        bool bFromSweep,
                        const FHitResult& SweepResult);

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

    bool bIsSprinting;
};

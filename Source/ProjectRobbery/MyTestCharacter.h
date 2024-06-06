#pragma once

#include "BasicCharacter.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "MyTestCharacter.generated.h"

UCLASS()
class PROJECTROBBERY_API AMyTestCharacter : public ABasicCharacter
{
    GENERATED_BODY()

public:
    AMyTestCharacter();

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void BeginPlay() override;              // BeginPlay 함수 선언 추가
    virtual void Tick(float DeltaTime) override;    // Tick 함수 선언 추가

    // Player health
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float Health;

    // Function to reduce health
    UFUNCTION()
    void ReduceHealth(float Amount);

    // Function to handle overlap events
    UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp,
                        class AActor* OtherActor,
                        class UPrimitiveComponent* OtherComp,
                        int32 OtherBodyIndex,
                        bool bFromSweep,
                        const FHitResult& SweepResult);

private:
    void MoveForward(float Value);
    void MoveRight(float Value);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FollowCamera;
};

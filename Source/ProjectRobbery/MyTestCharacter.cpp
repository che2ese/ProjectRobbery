#include "MyTestCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Door.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "YourAIController.h"
#include "DrawDebugHelpers.h"
#include "Camera.h"

// Sets default values
AMyTestCharacter::AMyTestCharacter()
{
    // CameraBoom을 초기화하고 RootComponent에 부착합니다.
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 800.0f;
    CameraBoom->bUsePawnControlRotation = false;
    CameraBoom->bDoCollisionTest = false;

    // 카메라를 캐릭터 위에 위치시킵니다.
    CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
    CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));    // -60도 피치로 탑다운 뷰 설정

    // FollowCamera를 초기화하고 CameraBoom에 부착합니다.
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;    // 카메라의 회전은 컨트롤러의 회전에 의존하지 않음

    GetCharacterMovement()->bOrientRotationToMovement = true;

    // Initialize health
    Health = 100.0f;
    RunHealth = 10.0f;    // 뛰는 동안 사용할 체력

    SprintDepletionRate = 2.0f;    // Shift 누를 때 달리면서 소모되는 체력 속도
    SprintRecoveryRate = 1.0f;     // Shift 뗄 때 체력이 회복되는 속도

    GetCharacterMovement()->MaxWalkSpeed = 300.0f;

    // Enable overlap events
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

    // Initialize sprinting
    bIsSprinting = false;

    FootstepAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FootstepAudio"));
    FootstepAudioComponent->SetupAttachment(RootComponent);    // FootstepAudioComponent를 RootComponent에 부착합니다.
    FootstepAudioComponent->bAutoActivate = false;    // 게임 시작 시 자동으로 활성화되지 않도록 설정합니다.
}

void AMyTestCharacter::BeginPlay()
{
    Super::BeginPlay();
}


// Called every frame
void AMyTestCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!bIsSprinting)
    {
        RecoverRunHealth(DeltaTime);
    }
    else
    {
        DepleteRunHealth(DeltaTime);
    }
}

// Called to bind functionality to input
void AMyTestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // 이동 함수들을 입력에 바인딩합니다.
    PlayerInputComponent->BindAxis("MoveForward", this, &AMyTestCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AMyTestCharacter::MoveRight);
    // Sprint 키 입력에 바인딩합니다.
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMyTestCharacter::StartSprinting);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMyTestCharacter::StopSprinting);
    PlayerInputComponent->BindAction("E", IE_Pressed, this, &AMyTestCharacter::UseCamera);
    PlayerInputComponent->BindAction("Q", IE_Pressed, this, &AMyTestCharacter::UseCoat);
}

void AMyTestCharacter::MoveForward(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        // 입력 방향을 얻습니다.
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRot(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
        AddMovementInput(-Direction, Value);
    }
}

void AMyTestCharacter::MoveRight(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        // 입력 방향을 얻습니다.
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRot(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
        AddMovementInput(-Direction, Value);
    }
}
void AMyTestCharacter::StartSprinting()
{
    if (bIsSprinting || RunHealth <= 0)
    {
        return;    // 이미 달리거나 RunHealth가 0 이하이면 더 이상 달리지 않음
    }
    bIsSprinting = true;
    GetCharacterMovement()->MaxWalkSpeed = 600.f;

    // Debug 메시지 추가
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Started Sprinting"));
    }
    if (FootstepAudioComponent)
    {
        FootstepAudioComponent->Play();
        PlaySoundEvent();
    }
}

void AMyTestCharacter::StopSprinting()
{
    if (!bIsSprinting)
    {
        return;    // 달리고 있지 않으면 무시
    }

    bIsSprinting = false;
    GetCharacterMovement()->MaxWalkSpeed = 300.f;

    if (FootstepAudioComponent && FootstepAudioComponent->IsPlaying())
    {
        FootstepAudioComponent->Stop();
    }
}

void AMyTestCharacter::ReduceHealth(float Amount)
{
    Health -= Amount;
    if (Health <= 0)
    {
        Destroy();
    }
    
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Damage"));
    }
}
bool AMyTestCharacter::HasKey(int32 num)
{
    for (int i = 0; i < Inventory.Num(); i++)
    {
        if (Inventory[i] == EItemType::Item_Key)
        {
            for (int j = 0; j < keySet.Num(); j++)
            {
                if (keySet[j] == num)
                {
                    return true;
                }
            }
            return false;
        }
    }
    return false;
}

void AMyTestCharacter::UseCamera()
{
    UE_LOG(LogTemp, Log, TEXT("Use Camera"));
    for (int i = 0; i < Inventory.Num(); i++)
    {
        if (Inventory[i] == EItemType::Item_Camera)
        {
            for (TActorIterator<ACamera> It(GetWorld()); It; ++It)
                It->enable = false;
        }
    }
}
void AMyTestCharacter::UseCoat()
{
    UE_LOG(LogTemp, Log, TEXT("Use Coat"));
    for (int i = 0; i < Inventory.Num(); i++)
    {
        if (Inventory[i] == EItemType::Item_Coat)
        {
            for (TActorIterator<AYourAIController> It(GetWorld()); It; ++It)
                It->coatActive = true;
        }
    }
}
void AMyTestCharacter::PlaySoundEvent()
{
    // 소리 이벤트를 생성하고 재생합니다.
    FHitResult HitResult;
    FVector StartLocation = GetActorLocation();
    FVector EndLocation =
        StartLocation + FVector(100.0f, 0.0f, 0.0f);    // 예시로 100 유닛 앞에 소리 이벤트를 생성합니다.

    // 소리 이벤트 생성
    MakeNoise(1.0f, this, StartLocation);

    // 디버그용으로 라인을 그립니다.
    DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 1.0f, 0, 1.0f);
}
void AMyTestCharacter::DepleteRunHealth(float DeltaTime)
{
    RunHealth -= SprintDepletionRate * DeltaTime;    // SprintDepletionRate마다 RunHealth를 감소시킴

    // 체력이 0 이하로 떨어졌을 때 달리기 중지
    if (RunHealth <= 0)
    {
        StopSprinting();
    }
}

void AMyTestCharacter::RecoverRunHealth(float DeltaTime)
{
    RunHealth += SprintRecoveryRate * DeltaTime;    // SprintRecoveryRate마다 RunHealth를 증가시킴

    // 최대 RunHealth를 초과하지 않도록 보정
    if (RunHealth > 10.0f)    // 최대 RunHealth 값
    {
        RunHealth = 10.0f;
    }
}

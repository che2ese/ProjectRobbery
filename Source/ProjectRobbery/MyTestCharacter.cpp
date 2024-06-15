#include "MyTestCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextBlock.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Door.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "YourAIController.h"
#include "Camera.h"
#include "NoiseActor.h"
#include "DrawDebugHelpers.h"
#include "ProjectRobberyGameMode.h"
#include "MainWidget.h"

// Sets default values
AMyTestCharacter::AMyTestCharacter()
{
    // CameraBoom�� �ʱ�ȭ�ϰ� RootComponent�� �����մϴ�.
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 800.0f;
    CameraBoom->bUsePawnControlRotation = false;
    CameraBoom->bDoCollisionTest = false;

    // ī�޶� ĳ���� ���� ��ġ��ŵ�ϴ�.
    CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 70.0f));
    CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));    // -60�� ��ġ�� ž�ٿ� �� ����

    // FollowCamera�� �ʱ�ȭ�ϰ� CameraBoom�� �����մϴ�.
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;    // ī�޶��� ȸ���� ��Ʈ�ѷ��� ȸ���� �������� ����

    GetCharacterMovement()->bOrientRotationToMovement = true;

    // Initialize health
    Health = 100.0f;
    RunHealth = 10.0f;    // �ٴ� ���� ����� ü��

    SprintDepletionRate = 2.0f;    // Shift ���� �� �޸��鼭 �Ҹ�Ǵ� ü�� �ӵ�
    SprintRecoveryRate = 1.0f;     // Shift �� �� ü���� ȸ���Ǵ� �ӵ�

    GetCharacterMovement()->MaxWalkSpeed = 300.0f;

    // Enable overlap events
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

    // Initialize sprinting
    bIsSprinting = false;

    noiseActive = false;

    FootstepAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FootstepAudio"));
    FootstepAudioComponent->SetupAttachment(RootComponent);    // FootstepAudioComponent를 RootComponent에 부착합니다.
    FootstepAudioComponent->bAutoActivate = false;    // 게임 시작 시 자동으로 활성화되지 않도록 설정합니다.
    FootstepAudioComponent->bAutoDestroy = false;    // 게임 시작 시 자동으로 활성화되지 않도록 설정합니다.

    DogAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("DogSound"));
    DogAudioComponent->SetupAttachment(RootComponent);    // FootstepAudioComponent를 RootComponent에 부착합니다.
    DogAudioComponent->bAutoActivate = false;    // 게임 시작 시 자동으로 활성화되지 않도록 설정합니다.
    DogAudioComponent->bAutoDestroy = false;    // 게임 시작 시 자동으로 활성화되지 않도록 설정합니다.

    NoiseAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("NoiseSound"));
    NoiseAudioComponent->SetupAttachment(RootComponent);    // FootstepAudioComponent를 RootComponent에 부착합니다.
    NoiseAudioComponent->bAutoActivate = false;    // 게임 시작 시 자동으로 활성화되지 않도록 설정합니다.
    NoiseAudioComponent->bAutoDestroy = false;    // 게임 시작 시 자동으로 활성화되지 않도록 설정합니다.

    point = 0;
}

void AMyTestCharacter::BeginPlay()
{
    Super::BeginPlay();

    AGameModeBase* curBase = GetWorld()->GetAuthGameMode();
    AProjectRobberyGameMode* curGameModeBase = Cast<AProjectRobberyGameMode>(curBase);

    curGameModeBase->mainUI->HealthText->SetText(FText::AsNumber((int)Health));
    curGameModeBase->mainUI->SteminaText->SetText(FText::AsNumber((int)RunHealth));
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

    // �̵� �Լ����� �Է¿� ���ε��մϴ�.
    PlayerInputComponent->BindAxis("MoveForward", this, &AMyTestCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AMyTestCharacter::MoveRight);
    // Sprint Ű �Է¿� ���ε��մϴ�.
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMyTestCharacter::StartSprinting);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMyTestCharacter::StopSprinting);
    // ������ ���Ű
    PlayerInputComponent->BindAction("E", IE_Pressed, this, &AMyTestCharacter::UseCamera);
    PlayerInputComponent->BindAction("Q", IE_Pressed, this, &AMyTestCharacter::UseCoat);
    PlayerInputComponent->BindAction("T", IE_Pressed, this, &AMyTestCharacter::UseNoise);
}

void AMyTestCharacter::MoveForward(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        // �Է� ������ ����ϴ�.
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

    AGameModeBase* curBase = GetWorld()->GetAuthGameMode();
    AProjectRobberyGameMode* curGameModeBase = Cast<AProjectRobberyGameMode>(curBase);
    curGameModeBase->mainUI->HealthText->SetText(FText::AsNumber((int)Health));

    if (Health <= 0)
    {
        curGameModeBase->ShowGameOverMenu();
        // Destroy();
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
            AGameModeBase* curBase = GetWorld()->GetAuthGameMode();
            AProjectRobberyGameMode* curGameModeBase = Cast<AProjectRobberyGameMode>(curBase);
            curGameModeBase->UseItems(Inventory[i]);
            Inventory[i] = EItemType::Item_None;
            break;
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
            AGameModeBase* curBase = GetWorld()->GetAuthGameMode();
            AProjectRobberyGameMode* curGameModeBase = Cast<AProjectRobberyGameMode>(curBase);
            curGameModeBase->UseItems(Inventory[i]);
            Inventory[i] = EItemType::Item_None;
            UE_LOG(LogTemp, Log, TEXT("Use Coat"));
            break;
        }
    }
}
void AMyTestCharacter::UseNoise()
{
    UE_LOG(LogTemp, Log, TEXT("Use Noise"));
    for (int i = 0; i < Inventory.Num(); i++)
    {
        if (Inventory[i] == EItemType::Item_Noise)
        {
            noiseActive = true;

            // NoiseActor 활성화 시 오디오 출력
            if (noiseActive)
            {
                if (NoiseAudioComponent)
                {
                    NoiseAudioComponent->Play();
                }
            }
            FVector myLoc = GetActorLocation();
            FRotator myRot = GetActorRotation();
            FActorSpawnParameters SpawnInfo;
            ANoiseActor* noise =
                GetWorld()->SpawnActor<ANoiseActor>(ANoiseActor::StaticClass(), myLoc, myRot, SpawnInfo);

            AGameModeBase* curBase = GetWorld()->GetAuthGameMode();
            AProjectRobberyGameMode* curGameModeBase = Cast<AProjectRobberyGameMode>(curBase);
            curGameModeBase->UseItems(Inventory[i]);
            Inventory[i] = EItemType::Item_None;
            UE_LOG(LogTemp, Log, TEXT("Use Noise"));
            break;
        }
    }
}
void AMyTestCharacter::PlaySoundEvent()
{
    FHitResult HitResult;
    FVector StartLocation = GetActorLocation();
    FVector EndLocation =
        StartLocation + FVector(100.0f, 0.0f, 0.0f);

    MakeNoise(1.0f, this, StartLocation);

    DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, 1.0f, 0, 1.0f);
}

void AMyTestCharacter::DepleteRunHealth(float DeltaTime)
{
    RunHealth -= SprintDepletionRate * DeltaTime; 

    if (RunHealth <= 0)
    {
        RunHealth = 0;
        StopSprinting();
    }

    AGameModeBase* curBase = GetWorld()->GetAuthGameMode();
    AProjectRobberyGameMode* curGameModeBase = Cast<AProjectRobberyGameMode>(curBase);
    curGameModeBase->mainUI->SteminaText->SetText(FText::AsNumber((int)RunHealth));
}

void AMyTestCharacter::RecoverRunHealth(float DeltaTime)
{
    RunHealth += SprintRecoveryRate * DeltaTime;   

    if (RunHealth > 10.0f)  
    {
        RunHealth = 10.0f;
    }

    AGameModeBase* curBase = GetWorld()->GetAuthGameMode();
    AProjectRobberyGameMode* curGameModeBase = Cast<AProjectRobberyGameMode>(curBase);
    curGameModeBase->mainUI->SteminaText->SetText(FText::AsNumber((int)RunHealth));
}

bool AMyTestCharacter::HasPoint()
{
    for (int i = 0; i < Inventory.Num(); i++)
    {
        if (Inventory[i] == EItemType::Item_Point)
        {
            return true;
        }
    }
    return false;
}
int AMyTestCharacter::GetPoint()
{
    for (int i = 0; i < Inventory.Num(); i++)
    {
        if (Inventory[i] == EItemType::Item_Point)
        {
            point++;
            UE_LOG(LogTemp, Log, TEXT("Point increased: %d"), point);
            Inventory[i] = EItemType::Item_None;    // 아이템을 사용한 것으로 처리
            break;    // 필요한 경우 찾았으면 반복문을 빠져나갈 수 있습니다.
        }
    }
    return point;
}
#include "MyTestCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

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

    GetCharacterMovement()->MaxWalkSpeed = 300.0f;

    // Enable overlap events
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

    // Initialize sprinting
    bIsSprinting = false;
}

void AMyTestCharacter::BeginPlay()
{
    Super::BeginPlay();
    GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyTestCharacter::OnOverlapBegin);
}


// Called every frame
void AMyTestCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
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
        // �Է� ������ ����ϴ�.
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRot(0, Rotation.Yaw, 0);
        const FVector Direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
        AddMovementInput(-Direction, Value);
    }
}
void AMyTestCharacter::StartSprinting()
{
    bIsSprinting = true;
    GetCharacterMovement()->MaxWalkSpeed = 600.f;
    // Debug �޽��� �߰�
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Started Sprinting"));
    }
}

void AMyTestCharacter::StopSprinting()
{
    bIsSprinting = false;
    GetCharacterMovement()->MaxWalkSpeed = 300.f;
}
void AMyTestCharacter::ReduceHealth(float Amount)
{
    Health -= Amount;
    if (Health <= 0)
    {
        // Handle player death
    }
    // Print "Damage" message to the screen
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Damage"));
    }
}

void AMyTestCharacter::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp,
                                      class AActor* OtherActor,
                                      class UPrimitiveComponent* OtherComp,
                                      int32 OtherBodyIndex,
                                      bool bFromSweep,
                                      const FHitResult& SweepResult)
{
    // Check if the overlap event is triggered
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Overlap Event Triggered"));
    }

    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        // Print name of the overlapping actor for debugging
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1,
                                             5.f,
                                             FColor::Yellow,
                                             FString::Printf(TEXT("Overlapping with: %s"), *OtherActor->GetName()));
        }

        // Check if the overlapping actor is the enemy
        if (OtherActor->ActorHasTag("Enemy"))
        {
            ReduceHealth(10.0f);

            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap with Enemy"));
            }
        }
    }
}

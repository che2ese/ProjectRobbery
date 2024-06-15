#include "Camera.h"

#include "../MyTestCharacter.h"
#include "../YourAIController.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACamera::ACamera()
{
    PrimaryActorTick.bCanEverTick = true;

    capsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
    SetRootComponent(capsuleComp);

    meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static mesh"));
    meshComp->SetupAttachment(capsuleComp);
}

// Called when the game starts or when spawned
void ACamera::BeginPlay()
{
    Super::BeginPlay();

    capsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ACamera::OnPlayerOverlap);
}

// Called every frame
void ACamera::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACamera::OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent,
                              AActor* OtherActor,
                              UPrimitiveComponent* OtherComp,
                              int32 OtherBodyIndex,
                              bool bFromSweep,
                              const FHitResult& SweepResult)
{
    AMyTestCharacter* player = Cast<AMyTestCharacter>(OtherActor);
    if (player != nullptr && enable)
    {
        UE_LOG(LogTemp, Log, TEXT("Player Caught!"));

        // 모든 AYourAIController 인스턴스에 대해 cameraActive를 true로 설정
        for (TActorIterator<AYourAIController> It(GetWorld()); It; ++It)
        {
            It->cameraActive = true;
            It->camera = this;
        }
    }
}

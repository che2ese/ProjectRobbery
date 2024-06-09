// Fill out your copyright notice in the Description page of Project Settings.

#include "Camera.h"
#include "../MyTestCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../YourAIController.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"

// Sets default values
ACamera::ACamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
        for (TActorIterator<AYourAIController> It(GetWorld()); It; ++It)
        {
            It->cameraActive = true;
            It->camera = this;
        }
        //군인도 위치로
    }
}


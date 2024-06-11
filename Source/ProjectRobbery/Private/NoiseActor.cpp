// Fill out your copyright notice in the Description page of Project Settings.


#include "NoiseActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../YourAIController.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
//#include "적들 전부"

// Sets default values
ANoiseActor::ANoiseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
    SetRootComponent(boxComp);

    meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static mesh"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> meshAsset(
        TEXT("/Game/OldWest/VOL3/Meshes/SM_Vintage_Clock_01a"));
    meshComp->SetStaticMesh(meshAsset.Object);
    meshComp->SetupAttachment(boxComp);
}

// Called when the game starts or when spawned
void ANoiseActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ANoiseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    countdown -= 1 * DeltaTime;
    if (countdown <= 0.1)
        Destroy();
    for (TActorIterator<AYourAIController> It(GetWorld()); It; ++It)
    {
        AActor::MakeNoiseImpl(this, 1.0f, It->GetPawn(), GetActorLocation(), 500.0f, TEXT("Noise Start"));
    }
}
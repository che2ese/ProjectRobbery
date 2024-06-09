// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemBase.h"

#include "../MyTestCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AItemBase::AItemBase()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
    SetRootComponent(boxComp);
    boxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));

    meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static mesh"));
    meshComp->SetupAttachment(boxComp);

    boxComp->SetCollisionProfileName(TEXT("OverlabOnlyPawn"));

    Item = EItemType::Item_None;
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
    Super::BeginPlay();

    boxComp->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::OnPlayerOverlap);
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AItemBase::OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent,
                                AActor* OtherActor,
                                UPrimitiveComponent* OtherComp,
                                int32 OtherBodyIndex,
                                bool bFromSweep,
                                const FHitResult& SweepResult)
{
    AMyTestCharacter* player = Cast<AMyTestCharacter>(OtherActor);
    if (player != nullptr)
    {
        player->Inventory.Add(Item);

        UE_LOG(LogTemp, Log, TEXT("Get Item"));
        Destroy();
    }
}
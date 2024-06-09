// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "../MyTestCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("EnterCollider"));
    SetRootComponent(boxComp);

    doorComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    doorComp->SetupAttachment(boxComp);

    doorCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("DoorCollider"));
    doorCollisionComp->SetupAttachment(doorComp);
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

    boxComp->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnPlayerOverlap);
    boxComp->OnComponentEndOverlap.AddDynamic(this, &ADoor::OffPlayerOverlap);
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}


void ADoor::OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent,
                            AActor* OtherActor,
                            UPrimitiveComponent* OtherComp,
                            int32 OtherBodyIndex,
                            bool bFromSweep,
                            const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Log, TEXT("%d Door"), doorNum);
    AMyTestCharacter* player = Cast<AMyTestCharacter>(OtherActor);
    if (player != nullptr && player->HasKey(doorNum))
    {
        OpenDoor();
    }
}

void ADoor::OffPlayerOverlap(UPrimitiveComponent* OverlappedComponent,
                            AActor* OtherActor,
                            UPrimitiveComponent* OtherComp,
                            int32 OtherBodyIndex)
{
    UE_LOG(LogTemp, Log, TEXT("%d Door"), doorNum);
    AMyTestCharacter* player = Cast<AMyTestCharacter>(OtherActor);
    if (player != nullptr)
    {
        CloseDoor();
    }
}

void ADoor::OpenDoor()
{
    if (!isOpenDoor)
    {
        UE_LOG(LogTemp, Log, TEXT("Open Door"));
        FRotator currentRot = GetActorRotation();
        doorComp->SetWorldRotation(FRotator(currentRot.Pitch, currentRot.Yaw - 90.0f, currentRot.Roll));
        isOpenDoor = true;
    }
}

void ADoor::CloseDoor()
{
    if (isOpenDoor)
    {
        UE_LOG(LogTemp, Log, TEXT("Close Door"));
        FRotator currentRot = GetActorRotation();
        doorComp->SetWorldRotation(FRotator(currentRot.Pitch, currentRot.Yaw, currentRot.Roll));
        isOpenDoor = false;
    }
}
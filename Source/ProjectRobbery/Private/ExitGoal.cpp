// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitGoal.h"
#include "../MyTestCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AExitGoal::AExitGoal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
    SetRootComponent(sphereComp);
    sphereComp->SetSphereRadius(50.0f);

    meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static mesh"));
    meshComp->SetupAttachment(sphereComp);
}

// Called when the game starts or when spawned
void AExitGoal::BeginPlay()
{
	Super::BeginPlay();
	
    sphereComp->OnComponentBeginOverlap.AddDynamic(this, &AExitGoal::OnPlayerOverlap);
}

// Called every frame
void AExitGoal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExitGoal::OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent,
                                AActor* OtherActor,
                                UPrimitiveComponent* OtherComp,
                                int32 OtherBodyIndex,
                                bool bFromSweep,
                                const FHitResult& SweepResult)
{
    AMyTestCharacter* player = Cast<AMyTestCharacter>(OtherActor);
    if (player != nullptr && player->HasPoint())
    {
        UE_LOG(LogTemp, Log, TEXT("Clear Stage"));
        //클리어 창 띄우기
    }
}
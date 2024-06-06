// Fill out your copyright notice in the Description page of Project Settings.


#include "YourAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "MyTestCharacter.h"

AYourAIController::AYourAIController() 
{
    PrimaryActorTick.bCanEverTick = true;

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

    SightConfig->SightRadius = AISightRadius;
    SightConfig->LoseSightRadius = AILoseSightRadius;
    SightConfig->PeripheralVisionAngleDegrees = AIFieldOfView;
    SightConfig->SetMaxAge(AISightAge);

    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

    GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
    GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AYourAIController::OnPawnDetected);
    GetPerceptionComponent()->ConfigureSense(*SightConfig);
}
void AYourAIController::BeginPlay()
{
    Super::BeginPlay();

    if (GetPerceptionComponent() != nullptr) 
    {
        UE_LOG(LogTemp, Warning, TEXT("All Systems Set!"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No Component"));
    }
}
void AYourAIController::OnPossess(APawn* Pawns)
{
    Super::OnPossess(Pawns);
}
void AYourAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    AMyTestCharacter* character = Cast<AMyTestCharacter>(GetPawn());

    if (bIsPlayerDetected == true)
    {
        AMyTestCharacter* player = Cast<AMyTestCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
        MoveToActor(player, 2.0f);
    }
}
FRotator AYourAIController::GetControlRotation() const
{
    if (GetPawn() == nullptr)
    {
        return FRotator(0.f, 0.f, 0.f);
    }
    return FRotator(0.f, GetPawn()->GetActorRotation().Yaw, 0.f);
}
void AYourAIController::OnPawnDetected(const TArray<AActor*> &DetectedPawns)
{
    for (size_t i = 0; i < DetectedPawns.Num(); i++)
    {
        DistanceToPlayer = GetPawn()->GetDistanceTo(DetectedPawns[i]);

        UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), DistanceToPlayer);
    }
    bIsPlayerDetected = true;
}
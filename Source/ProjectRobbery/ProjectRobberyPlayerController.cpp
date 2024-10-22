// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectRobberyPlayerController.h"

#include "ProjectRobbery.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine/World.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "ProjectRobberyCharacter.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"

AProjectRobberyPlayerController::AProjectRobberyPlayerController()
{
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AProjectRobberyPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    // keep updating the destination every tick while desired
    if (bMoveToMouseCursor)
    {
        MoveToMouseCursor();
    }
}

void AProjectRobberyPlayerController::SetupInputComponent()
{
    // set up gameplay key bindings
    Super::SetupInputComponent();

    InputComponent->BindAction("SetDestination",
                               IE_Pressed,
                               this,
                               &AProjectRobberyPlayerController::OnSetDestinationPressed);
    InputComponent->BindAction("SetDestination",
                               IE_Released,
                               this,
                               &AProjectRobberyPlayerController::OnSetDestinationReleased);

    // support touch devices
    InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AProjectRobberyPlayerController::MoveToTouchLocation);
    InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AProjectRobberyPlayerController::MoveToTouchLocation);

    InputComponent->BindAction("ResetVR", IE_Pressed, this, &AProjectRobberyPlayerController::OnResetVR);
}

void AProjectRobberyPlayerController::OnResetVR()
{
    UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AProjectRobberyPlayerController::MoveToMouseCursor()
{
}

void AProjectRobberyPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
    FVector2D ScreenSpaceLocation(Location);

    // Trace to see what is under the touch location
    FHitResult HitResult;
    GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
    if (HitResult.bBlockingHit)
    {
        // We hit something, move there
        SetNewMoveDestination(HitResult.ImpactPoint);
    }
}

void AProjectRobberyPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
    APawn* const MyPawn = GetPawn();
    if (MyPawn)
    {
        float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

        // We need to issue move command only if far enough in order for walk animation to play correctly
        if ((Distance > 5.0f))
        {
            UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
        }
    }
}

void AProjectRobberyPlayerController::OnSetDestinationPressed()
{
    // set flag to keep updating destination until released
    bMoveToMouseCursor = true;
}

void AProjectRobberyPlayerController::OnSetDestinationReleased()
{
    // clear flag to indicate we should stop updating the destination
    bMoveToMouseCursor = false;
}

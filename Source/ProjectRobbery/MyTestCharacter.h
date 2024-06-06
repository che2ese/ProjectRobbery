// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicCharacter.h"
#include "MyTestCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTROBBERY_API AMyTestCharacter : public ABasicCharacter
{
	GENERATED_BODY()
public:
    AMyTestCharacter();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FollowCamera;
};

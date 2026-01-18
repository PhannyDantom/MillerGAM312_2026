// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "PlayerChar.generated.h"

UCLASS()
class MILLER_GAM312_2026_API APlayerChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION()
		void MoveForward(float axisValue); //Move Foward and Backwards

	UFUNCTION()
		void MoveRight(float axisValue); //Move Left and Right

	UFUNCTION()
		void StartJump(); //Begin Jumping

	UFUNCTION()
		void StopJump(); //Stop Jumping

	UFUNCTION()
		void FindObject(); //Interact

	UPROPERTY(VisibleAnywhere) //Player Camera
		UCameraComponent* PlayerCamComp;

};

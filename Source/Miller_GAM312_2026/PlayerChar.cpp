// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChar.h"

// Sets default values
APlayerChar::APlayerChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Camera Component Setup
	PlayerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Cam"));

	//Camera Attachment to Player Mesh/Head
	PlayerCamComp->SetupAttachment(GetMesh(), "head");

	//Share Rotation with Controller
	PlayerCamComp->bUsePawnControlRotation = true;


}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerChar::MoveForward); //Move Forward and Backwards (W/S Keys)
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerChar::MoveRight); //Move Right and Left (A/D Keys)
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerChar::AddControllerPitchInput); //Camera Moves Up/Down with Mouse
	PlayerInputComponent->BindAxis("Turn", this, &APlayerChar::AddControllerPitchInput); //Camera Moves Left/Right with Mouse
	PlayerInputComponent->BindAction("JumpEvent", IE_Pressed, this, &APlayerChar::StartJump); //Jump When Spacebar is Pressed
	PlayerInputComponent->BindAction("JumpEvent", IE_Released, this, &APlayerChar::StopJump); //Stop When Spacebar is Released
}

void APlayerChar::MoveForward(float axisValue)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X); //W/S Key Functionality
	AddMovementInput(Direction, axisValue);
}

void APlayerChar::MoveRight(float axisValue)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y); //A/D Key Functionality
	AddMovementInput(Direction, axisValue);
}

void APlayerChar::StartJump() //If Spacebar is Pressed, Jump
{
	bPressedJump = true;
}

void APlayerChar::StopJump() //If Spacebar is Released, End Jump
{
	bPressedJump = false;
}

void APlayerChar::FindObject() //TBA -- Will Be RMB to Interact
{
}


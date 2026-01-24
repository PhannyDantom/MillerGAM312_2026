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

	//Resource Arrays
	ResourcesArray.SetNum(3);
	ResourcesNameArray.Add(TEXT("Wood"));
	ResourcesNameArray.Add(TEXT("Stone"));
	ResourcesNameArray.Add(TEXT("Berry"));

	//Building Array
	BuildingArray.SetNum(3);


}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle StatsTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(StatsTimerHandle, this, & APlayerChar::DecreaseStats, 2.0f, true); //Stats Auto Decrease Every 2 Sec
	
}

// Called every frame
void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isBuilding) {
		if (spawnedPart) {
			FVector StartLocation = PlayerCamComp->GetComponentLocation();
			FVector Direction = PlayerCamComp->GetForwardVector() * 400.0f;
			FVector EndLocation = StartLocation + Direction;
			spawnedPart->SetActorLocation(EndLocation);
		}
	}

}

// Called to bind functionality to input
void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerChar::MoveForward); //Move Forward and Backwards (W/S Keys)
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerChar::MoveRight); //Move Right and Left (A/D Keys)
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerChar::AddControllerPitchInput); //Camera Moves Up/Down with Mouse
	PlayerInputComponent->BindAxis("Turn", this, &APlayerChar::AddControllerYawInput); //Camera Moves Left/Right with Mouse
	PlayerInputComponent->BindAction("JumpEvent", IE_Pressed, this, &APlayerChar::StartJump); //Jump When Spacebar is Pressed
	PlayerInputComponent->BindAction("JumpEvent", IE_Released, this, &APlayerChar::StopJump); //Stop When Spacebar is Released
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerChar::FindObject); //Interact with Object when Clicked
	PlayerInputComponent->BindAction("RotPart", IE_Pressed, this, &APlayerChar::RotateBuilding); //Spin Building 90
}


//PLAYER MOVEMENTS

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

void APlayerChar::FindObject() 
{
	FHitResult HitResult;
	FVector StartLocation = PlayerCamComp->GetComponentLocation(); //Object Location
	FVector Direction = PlayerCamComp->GetForwardVector() * 800.0f;
	FVector EndLocation = StartLocation + Direction;

	FCollisionQueryParams QueryParams; //Collisions
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnFaceIndex = true;


	if (!isBuilding) {

		//Collect Resource Actor, Destroy When 0
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams)) {
			AResource_M* HitResource = Cast<AResource_M>(HitResult.GetActor());
			if (Stamina > 5.0f) { //Can Only Collect With Stamina >5

				if (HitResource) {
					FString hitName = HitResource->resourceName;
					int resourceValue = HitResource->resourceAmount;

					HitResource->totalResource = HitResource->totalResource - resourceValue;

					if (HitResource->totalResource > resourceValue) {
						GiveResource(resourceValue, hitName);

						check(GEngine != nullptr);
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Collected"));

						UGameplayStatics::SpawnDecalAtLocation(GetWorld(), hitDecal, FVector(10.0f, 10.0f, 10.0f), HitResult.Location, FRotator(-90, 0, 0), 2.0f);

						SetStamina(-5.0f); //Depletes Stamina Per Hit
					}
					else {
						HitResource->Destroy();
						check(GEngine != nullptr);
						GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Resource Depleted"));
					}
				}
			}
		}
	}
	else {
		isBuilding = false;
	}
}

//PLAYER STATS

void APlayerChar::SetHealth(float amount) //Add or Subtract Health Within 0-100
{
	if (Health + amount < 100) {
		Health = Health + amount;
	}
}

void APlayerChar::SetHunger(float amount) //Add or Subtract Hunger Within 0-100
{
	if (Hunger + amount < 100) {
		Hunger = Hunger + amount;
	}
}

void APlayerChar::SetStamina(float amount) //Add or Subtract Stamina Within 0-100
{
	if (Stamina + amount < 100) {
		Stamina = Stamina + amount;
	}
}

void APlayerChar::DecreaseStats()
{
	if (Hunger > 0) {
		SetHunger(-1.0f); //Auto Hunger Decrease
	}

	SetStamina(10.0f); //Auto Stamina Renewal

	if (Hunger == 0) { //If Hunger Stat is 0, Decrease Health
		SetHealth(-3.0f);
	}
}

//COLLECT RESOURCES TO INVENTORY

void APlayerChar::GiveResource(float amount, FString resourceType)
{
	if (resourceType == "Wood") {
		ResourcesArray[0] = ResourcesArray[0] + amount;

	}

	if (resourceType == "Stone") {
		ResourcesArray[1] = ResourcesArray[1] + amount;

	}

	if (resourceType == "Berry") {
		ResourcesArray[2] = ResourcesArray[2] + amount;

	}
}

void APlayerChar::UpdateResources(float woodAmount, float stoneAmount, FString buildingObject) //Stores Material Amounts 
{
	if (woodAmount <= ResourcesArray[0]) {
		if (stoneAmount <= ResourcesArray[1]) {
			ResourcesArray[0] = ResourcesArray[0] - woodAmount;
			ResourcesArray[1] = ResourcesArray[1] - stoneAmount;

			if (buildingObject == "Wall") {
				BuildingArray[0] = BuildingArray[0] + 1;
			}

			if (buildingObject == "Floor") {
				BuildingArray[1] = BuildingArray[1] + 1;
			}

			if (buildingObject == "Ceiling") {
				BuildingArray[2] = BuildingArray[2] + 1;
			}
		}
	}
}

void APlayerChar::SpawnBuilding(int buildingID, bool& isSuccess) //Creates Building with Stored Material
{
	if (!isBuilding) {
		if (BuildingArray[buildingID] >= 1) {
			isBuilding = true;
			FActorSpawnParameters SpawnParams;
			FVector StartLocation = PlayerCamComp->GetComponentLocation();
			FVector Direction = PlayerCamComp->GetForwardVector() * 400.0f;
			FVector EndLocation = StartLocation + Direction;
			FRotator myRot(0, 0, 0);

			BuildingArray[buildingID] = BuildingArray[buildingID] - 1;

			spawnedPart = GetWorld()->SpawnActor<ABuildingPart>(BuildPartClass, EndLocation, myRot, SpawnParams);

			isSuccess = true;
		}

		isSuccess = false;
	}

}

void APlayerChar::RotateBuilding() //Rotate 90
{
	if (isBuilding) {
		spawnedPart->AddActorWorldRotation(FRotator(0, 90, 0));
	}
}


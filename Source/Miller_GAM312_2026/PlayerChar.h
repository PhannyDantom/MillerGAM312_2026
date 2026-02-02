// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Resource_M.h"
#include "Kismet/GameplayStatics.h"
#include "BuildingPart.h"
#include "PlayerWidget.h"
#include "ObjectiveWidget.h"
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

	//MOVEMENTS

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

	//PLAYER STATS

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats") //Health Set at 100
		float Health = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats") //Hunger Set at 100 
		float Hunger = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats") //Stamina Set at 100
		float Stamina = 100.0f;

	UFUNCTION(BlueprintCallable)
		void SetHealth(float amount); //Allows Health to Change

	UFUNCTION(BlueprintCallable) //Allows Hunger to Change
		void SetHunger(float amount);

	UFUNCTION(BlueprintCallable) //Allows Stamina to Change
		void SetStamina(float amount);

	UFUNCTION() //Auto Decreases All Stats
		void DecreaseStats();

	//INVENTORY RESOURCES

	UPROPERTY(EditAnywhere, Category = "Resources") //Wood Item
		int Wood;

	UPROPERTY(EditAnywhere, Category = "Resources") //Stone Item
		int Stone;

	UPROPERTY(EditAnywhere, Category = "Resources") //Berry Item
		int Berry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
		TArray<int> ResourcesArray;

	UPROPERTY(EditAnywhere, Category = "Resources")
		TArray<FString> ResourcesNameArray;

	UPROPERTY(EditAnywhere, Category = "HitMarker") //Interact Decal
		UMaterialInterface* hitDecal;

	UFUNCTION()
		void GiveResource(float amount, FString resourceType);

	//BUILDING

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Supplies") //Collects Building Material
		TArray<int> BuildingArray;

	UPROPERTY()
		bool isBuilding;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<ABuildingPart> BuildPartClass;

	UPROPERTY()
		ABuildingPart* spawnedPart;

	UFUNCTION(BlueprintCallable) //Add or Use Materials
		void UpdateResources(float woodAmount, float stoneAmount, FString buildingObject);

	UFUNCTION(BlueprintCallable) //Create Building 
		void SpawnBuilding(int buildingID, bool& isSuccess);

	UFUNCTION() //Rotate 90
		void RotateBuilding();

	//PLAYER WIDGET

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UPlayerWidget* playerUI;

	//OBJECT WIDGET

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UObjectiveWidget* objWidget;

	UPROPERTY()
		float objectsBuilt;

	UPROPERTY()
		float matsCollected;
};

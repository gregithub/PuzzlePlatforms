// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API AMovingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()

public:
	AMovingPlatform();

	virtual void BeginPlay() override;

protected:

	virtual void Tick(float DeltaTime) override;

	void MovePlatfrom(float DeltaTime);

	


private:

	UPROPERTY(EditAnywhere)
	float speed = 10;

	
};

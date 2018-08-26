// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include"MenuSystem/MenuInterface.h"
#include "PuzzlePlatformsGameInstance.generated.h"
class UUserWidget;


UCLASS()
class PUZZLEPLATFORMS_API UPuzzlePlatformsGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:

	UPuzzlePlatformsGameInstance(const FObjectInitializer & ObjectInitializer);
	virtual void Init();
	
	
	UFUNCTION(Exec)
		void Host() override;
	UFUNCTION(Exec)
		void Join(const FString& IPAdress) override;
	UFUNCTION(Exec)
		void LoadMainMenu() override;
	UFUNCTION(BlueprintCallable)
		void LoadMenu();
	UFUNCTION(BluePrintCallable)
		void LoadInGameMenu();

private:
	TSubclassOf<UUserWidget> MainMenuClass;
	TSubclassOf<UUserWidget> GameMenuClass;

	class UMainMenu * Menu;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include"MenuSystem/MenuInterface.h"
#include"OnlineSubsystem.h"
#include"OnlineSessionInterface.h"
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
		void Join(uint32 Index) override;
	UFUNCTION(Exec)
		void LoadMainMenu() override;
	UFUNCTION(BlueprintCallable)
		void LoadMenuWidget();
	UFUNCTION(BluePrintCallable)
		void LoadInGameMenu();

	void RefreshServerList() override;

private:
	TSubclassOf<UUserWidget> MainMenuClass;
	TSubclassOf<UUserWidget> GameMenuClass;

	class UMainMenu * Menu;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession();
};


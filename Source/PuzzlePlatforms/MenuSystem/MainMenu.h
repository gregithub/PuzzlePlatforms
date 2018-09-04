// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

USTRUCT()
struct FServerData {
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 TotalPlayers;
	FString HostUsername;
};
/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()

public:
	UMainMenu(const FObjectInitializer & ObjectInitializer);

	void SetServerList(TArray<FServerData> ServerData);

	void SelectIndex(uint32 Index);
private:
	TSubclassOf<UUserWidget> ServerRowClass;


	UPROPERTY(meta = (BindWidget))
		class UButton* HostButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* JoinButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* BackButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* QuitButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* OpenHostButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* CancelHostMenuButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* ConnectButton;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
		class UWidget* JoinMenu;
	UPROPERTY(meta = (BindWidget))
		class UWidget* HostMenu;
	UPROPERTY(meta = (BindWidget))
		class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* ServerHostName;
	


	UPROPERTY(meta = (BindWidget))
		class UPanelWidget* ServerList;


	UFUNCTION()
		void HostServer();
	UFUNCTION()
		void Connect();
	UFUNCTION()
		void OpenHostMenu();
	UFUNCTION()
		void OpenJoinMenu();
	UFUNCTION()
		void OpenMainMenu();
	UFUNCTION()
		void QuitGame();

	TOptional<uint32>SelectedIndex;

	void UpadateChildren();
protected:

	virtual bool Initialize() override;
};

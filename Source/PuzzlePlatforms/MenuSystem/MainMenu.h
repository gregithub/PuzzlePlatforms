// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLEPLATFORMS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMenuInterface(IMenuInterface* MenuInterface);

	void Setup();
	
	void Teardown();
private:
	UPROPERTY(meta = (BindWidget))
		class UButton* HostButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* JoinButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* BackButton;
	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* MenuSwitcher;
	UPROPERTY(meta = (BindWidget))
		class UWidget* JoinMenu;
	UPROPERTY(meta = (BindWidget))
		class UWidget* MainMenu;
	UPROPERTY(meta = (BindWidget))
		class UButton* ConnectButton;
	
	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* IPTextBox;


	UFUNCTION()
		void HostServer();
	UFUNCTION()
		void Connect();
	UFUNCTION()
		void OpenJoinMenu();
	UFUNCTION()
		void OpenMainMenu();
	
	IMenuInterface* MenuInterface;
protected:

	virtual bool Initialize() override;
};

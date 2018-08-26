// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"
#include"Components/Button.h"


bool UInGameMenu::Initialize() {
	bool Success = Super::Initialize();
	if (!Success) return false;


	if (!ensure(CancelButton != nullptr)) return false;
	CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::Cancel);

	if (!ensure(MainMenuButton != nullptr)) return false;
	MainMenuButton->OnClicked.AddDynamic(this, &UInGameMenu::BackToMenu);



	return true;
}

void UInGameMenu::Cancel() {
	Teardown();
}

void UInGameMenu::BackToMenu() {
	if (MenuInterface != nullptr) {
		Teardown();
		MenuInterface->LoadMainMenu();
	}

}
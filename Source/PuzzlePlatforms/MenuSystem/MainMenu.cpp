// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include"Components/Button.h"

bool UMainMenu::Initialize() {
	bool Success = Super::Initialize();
	if (!Success) return false;


	if (!ensure(Host != nullptr)) return false;
	Host->OnClicked.AddDynamic(this, &UMainMenu::HostServer);


	return true;
}

void UMainMenu::Setup() {
	this->AddToViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	APlayerController* PlayerControler = World->GetFirstPlayerController();
	if (!ensure(PlayerControler != nullptr)) return;

	FInputModeUIOnly InputMode;

	InputMode.SetWidgetToFocus(this->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerControler->SetInputMode(InputMode);

	PlayerControler->bShowMouseCursor = true;

}
void UMainMenu::Teardown() {
	this->RemoveFromViewport();
	
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	APlayerController* PlayerControler = World->GetFirstPlayerController();
	if (!ensure(PlayerControler != nullptr)) return;

	FInputModeGameOnly InputMode;
	PlayerControler->SetInputMode(InputMode);

	PlayerControler->bShowMouseCursor = false;

}

void UMainMenu::HostServer() {
	if (MenuInterface != nullptr) {
		MenuInterface->Host();
	}
}

void UMainMenu::SetMenuInterface(IMenuInterface* MenuInterface) {
	this->MenuInterface = MenuInterface;
}
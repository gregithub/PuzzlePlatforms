// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include"Components/Button.h"
#include"Components/WidgetSwitcher.h"
#include"Components/EditableTextBox.h"

bool UMainMenu::Initialize() {
	bool Success = Super::Initialize();
	if (!Success) return false;


	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(BackButton != nullptr)) return false;
	BackButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(ConnectButton != nullptr)) return false;
	ConnectButton->OnClicked.AddDynamic(this, &UMainMenu::Connect);


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
void UMainMenu::Connect() {
	if (MenuInterface != nullptr) {
		if (!ensure(IPTextBox != nullptr)) return;
		const FString& Address = IPTextBox->GetText().ToString();
		MenuInterface->Join(Address);
	}
}
void UMainMenu::OpenJoinMenu() {
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
}
void UMainMenu::OpenMainMenu() {
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);

}
void UMainMenu::SetMenuInterface(IMenuInterface* MenuInterface) {
	this->MenuInterface = MenuInterface;
}

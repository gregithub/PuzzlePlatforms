// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuWidget.h"




void UMenuWidget::SetMenuInterface(IMenuInterface* MenuInterface) {
	this->MenuInterface = MenuInterface;
}

void UMenuWidget::Setup() {
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
void UMenuWidget::Teardown() {
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	APlayerController* PlayerControler = World->GetFirstPlayerController();
	if (!ensure(PlayerControler != nullptr)) return;

	FInputModeGameOnly InputMode;
	PlayerControler->SetInputMode(InputMode);

	PlayerControler->bShowMouseCursor = false;

}
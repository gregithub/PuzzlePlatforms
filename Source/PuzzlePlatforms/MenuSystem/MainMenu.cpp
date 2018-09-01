// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "UObject/ConstructorHelpers.h"
#include"Components/Button.h"
#include"Components/WidgetSwitcher.h"
#include"Components/EditableTextBox.h"
#include"Components/TextBlock.h"

#include"ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer) {
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;
	ServerRowClass = ServerRowBPClass.Class;

}


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

	if (!ensure(QuitButton != nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);


	return true;
}


void UMainMenu::HostServer() {
	if (MenuInterface != nullptr) {
		MenuInterface->Host();
	}
}
void UMainMenu::SetServerList(TArray<FString> ServerNames) {
	
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;
	
	ServerList->ClearChildren();

	for (const FString& ServerName : ServerNames) {
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		Row->ServerName->SetText(FText::FromString(ServerName));

		ServerList->AddChild(Row);
	}
}

void UMainMenu::Connect() {
	if (MenuInterface != nullptr) {
		//if (!ensure(IPTextBox != nullptr)) return;
		//const FString& Address = IPTextBox->GetText().ToString();
		MenuInterface->Join("");
	
	}
}
void UMainMenu::OpenJoinMenu() {
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(JoinMenu);
	if(MenuInterface!=nullptr){
		MenuInterface->RefreshServerList();
	}
}
void UMainMenu::OpenMainMenu() {
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(MainMenu);

}
void UMainMenu::QuitGame() {
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	APlayerController* PlayerControler = World->GetFirstPlayerController();
	if (!ensure(PlayerControler != nullptr)) return;

	PlayerControler->ConsoleCommand("quit");
}
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

	if (!ensure(OpenHostButton != nullptr)) return false;
	OpenHostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);

	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(BackButton != nullptr)) return false;
	BackButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
	if (!ensure(CancelHostMenuButton != nullptr)) return false;
	CancelHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(ConnectButton != nullptr)) return false;
	ConnectButton->OnClicked.AddDynamic(this, &UMainMenu::Connect);

	if (!ensure(QuitButton != nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);


	return true;
}


void UMainMenu::HostServer() {
	if (MenuInterface != nullptr) {
		FString ServerName = ServerHostName->Text.ToString();
		MenuInterface->Host(ServerName);
	}
}
void UMainMenu::SetServerList(TArray<FServerData> ServerData) {
	
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;
	
	ServerList->ClearChildren();

	uint32 i = 0;
	for (const FServerData& InServerData : ServerData) {
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		Row->ServerName->SetText(FText::FromString(InServerData.Name));
		Row->HostUser->SetText(FText::FromString(InServerData.HostUsername));
		Row->ConnectionFraction->SetText(FText::FromString(
			FString::Printf(TEXT("%d/%d"),InServerData.CurrentPlayers,InServerData.TotalPlayers)));
		
		Row->Setup(this, i);
		++i;

		ServerList->AddChild(Row);
	}
}
void UMainMenu::SelectIndex(uint32 Index) {
	SelectedIndex = Index;
	UpadateChildren();
}

void UMainMenu::UpadateChildren() {
	for (int32 i = 0; i < ServerList->GetChildrenCount();++i) {
		UServerRow * Row = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (Row != nullptr) {
			Row->Selected = (SelectedIndex.IsSet() && SelectedIndex.GetValue()==i);
		}
	}
}

void UMainMenu::Connect() {
	if (SelectedIndex.IsSet()) {
		UE_LOG(LogTemp, Warning, TEXT("Selected index %d"),SelectedIndex.GetValue());
		MenuInterface->Join(SelectedIndex.GetValue());
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("SelectedIndex is not set"));
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
void UMainMenu::OpenHostMenu() {
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(HostMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(HostMenu);
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
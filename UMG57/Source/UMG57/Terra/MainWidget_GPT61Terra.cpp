// Model: GPT-6.1 Terra; reasoning: Medium
#include "Terra/MainWidget_GPT61Terra.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
void UMainWidget_GPT61Terra::NativeConstruct(){ Super::NativeConstruct(); OpenButton->OnClicked.AddDynamic(this,&ThisClass::OpenPopup); }
void UMainWidget_GPT61Terra::OpenPopup(){ auto C=LoadClass<UUserWidget>(nullptr,TEXT("/Game/UI/Terra/WBP_Popup_GPT61Terra.WBP_Popup_GPT61Terra_C")); auto* P=GetOwningPlayer(); if(C&&P){ auto* W=CreateWidget<UUserWidget>(P,C); W->AddToViewport(100); FInputModeUIOnly M; M.SetWidgetToFocus(W->TakeWidget()); P->SetInputMode(M); } }
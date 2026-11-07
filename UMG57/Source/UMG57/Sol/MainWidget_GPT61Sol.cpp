// Model: GPT-6.1 Sol; reasoning: Medium
#include "Sol/MainWidget_GPT61Sol.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
void UMainWidget_GPT61Sol::NativeConstruct(){ Super::NativeConstruct(); OpenButton->OnClicked.AddDynamic(this,&ThisClass::OpenPopup); }
void UMainWidget_GPT61Sol::OpenPopup(){ auto C=LoadClass<UUserWidget>(nullptr,TEXT("/Game/UI/Sol/WBP_Popup_GPT61Sol.WBP_Popup_GPT61Sol_C")); auto* P=GetOwningPlayer(); if(C&&P){ auto* W=CreateWidget<UUserWidget>(P,C); W->AddToViewport(100); FInputModeUIOnly M; M.SetWidgetToFocus(W->TakeWidget()); P->SetInputMode(M); } }
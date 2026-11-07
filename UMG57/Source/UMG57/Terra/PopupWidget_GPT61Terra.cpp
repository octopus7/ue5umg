// Model: GPT-6.1 Terra; reasoning: Medium
#include "Terra/PopupWidget_GPT61Terra.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
void UPopupWidget_GPT61Terra::NativeConstruct(){ Super::NativeConstruct(); CloseButton->OnClicked.AddDynamic(this,&ThisClass::ClosePopup); CloseButton->SetKeyboardFocus(); }
void UPopupWidget_GPT61Terra::NativeTick(const FGeometry& G,float D){ Super::NativeTick(G,D); if(auto* P=GetOwningPlayer()) if(!HasUserFocusedDescendants(P)&&!HasUserFocus(P)) CloseButton->SetUserFocus(P); }
void UPopupWidget_GPT61Terra::ClosePopup(){ auto* P=GetOwningPlayer(); RemoveFromParent(); if(P){ FInputModeGameAndUI Mode; P->SetInputMode(Mode); } }
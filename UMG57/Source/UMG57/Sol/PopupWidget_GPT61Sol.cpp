// Model: GPT-6.1 Sol; reasoning: Medium
#include "Sol/PopupWidget_GPT61Sol.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
void UPopupWidget_GPT61Sol::NativeConstruct(){ Super::NativeConstruct(); CloseButton->OnClicked.AddDynamic(this,&ThisClass::ClosePopup); CloseButton->SetKeyboardFocus(); }
void UPopupWidget_GPT61Sol::NativeTick(const FGeometry& G,float D){ Super::NativeTick(G,D); if(auto* P=GetOwningPlayer()) if(!HasUserFocusedDescendants(P)&&!HasUserFocus(P)) CloseButton->SetUserFocus(P); }
void UPopupWidget_GPT61Sol::ClosePopup(){ auto* P=GetOwningPlayer(); RemoveFromParent(); if(P){ FInputModeGameAndUI Mode; P->SetInputMode(Mode); } }
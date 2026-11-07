// Model: GPT-6.1 Terra; reasoning: Medium
#include "Terra/PopupWidget_GPT61Terra.h"
#include "Components/Button.h"
void UPopupWidget_GPT61Terra::NativeConstruct()
{
    Super::NativeConstruct();
    SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    CloseButton->OnClicked.AddUniqueDynamic(this, &ThisClass::ClosePopup);
}
void UPopupWidget_GPT61Terra::NativeDestruct()
{
    CloseButton->OnClicked.RemoveDynamic(this, &ThisClass::ClosePopup);
    Super::NativeDestruct();
}
void UPopupWidget_GPT61Terra::ClosePopup() { RemoveFromParent(); }

// Model: GPT-6.1 Sol; reasoning: Medium
#include "Sol/PopupWidget_GPT61Sol.h"
#include "Components/Button.h"
void UPopupWidget_GPT61Sol::NativeConstruct()
{
    Super::NativeConstruct();
    SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    if (CloseButton) CloseButton->OnClicked.AddUniqueDynamic(this, &ThisClass::ClosePopup);
}
void UPopupWidget_GPT61Sol::ClosePopup()
{
    RemoveFromParent();
}
void UPopupWidget_GPT61Sol::NativeDestruct()
{
    if (CloseButton) CloseButton->OnClicked.RemoveDynamic(this, &ThisClass::ClosePopup);
    Super::NativeDestruct();
}

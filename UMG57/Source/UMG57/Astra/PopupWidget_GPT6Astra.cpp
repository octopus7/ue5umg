// Actual model: GPT-6 Astra; reasoning effort: Medium.
#include "Astra/PopupWidget_GPT6Astra.h"
#include "Components/Button.h"

void UPopupWidget_GPT6Astra::NativeConstruct()
{
    Super::NativeConstruct();
    CloseButton->OnClicked.AddUniqueDynamic(this, &ThisClass::ClosePopup);
}

void UPopupWidget_GPT6Astra::ClosePopup()
{
    RemoveFromParent();
    OnClosed.ExecuteIfBound();
}

void UPopupWidget_GPT6Astra::NativeDestruct()
{
    CloseButton->OnClicked.RemoveDynamic(this, &ThisClass::ClosePopup);
    Super::NativeDestruct();
}

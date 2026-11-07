// Model: GPT-6.1 Sol; reasoning: Medium
#include "Sol/MainWidget_GPT61Sol.h"
#include "Components/Button.h"
void UMainWidget_GPT61Sol::NativeConstruct()
{
    Super::NativeConstruct();
    SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    OpenButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OpenPopup);
}
void UMainWidget_GPT61Sol::OpenPopup()
{
    if (IsValid(ActivePopup) && ActivePopup->IsInViewport()) return;
    ActivePopup = nullptr;
    if (!GetOwningPlayer()) return;
    const auto PopupClass = LoadClass<UUserWidget>(nullptr,
        TEXT("/Game/UI/Sol/WBP_Popup_GPT61Sol.WBP_Popup_GPT61Sol_C"));
    if (!PopupClass) return;
    ActivePopup = CreateWidget<UUserWidget>(GetOwningPlayer(), PopupClass);
    if (ActivePopup) ActivePopup->AddToViewport(100);
}
void UMainWidget_GPT61Sol::NativeDestruct()
{
    if (OpenButton) OpenButton->OnClicked.RemoveDynamic(this, &ThisClass::OpenPopup);
    if (IsValid(ActivePopup)) ActivePopup->RemoveFromParent();
    ActivePopup = nullptr;
    Super::NativeDestruct();
}

// Model: GPT-6.1 Terra; reasoning: Medium
#include "Terra/MainWidget_GPT61Terra.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"

void UMainWidget_GPT61Terra::NativeConstruct()
{
    Super::NativeConstruct();
    SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    OpenButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OpenPopup);
}

void UMainWidget_GPT61Terra::NativeDestruct()
{
    OpenButton->OnClicked.RemoveDynamic(this, &ThisClass::OpenPopup);
    if (ActivePopup) { ActivePopup->RemoveFromParent(); ActivePopup = nullptr; }
    Super::NativeDestruct();
}

void UMainWidget_GPT61Terra::OpenPopup()
{
    if (ActivePopup && ActivePopup->IsInViewport()) return;
    ActivePopup = nullptr;
    const auto PopupClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/UI/Terra/WBP_Popup_GPT61Terra.WBP_Popup_GPT61Terra_C"));
    if (PopupClass && GetOwningPlayer())
    {
        ActivePopup = CreateWidget<UUserWidget>(GetOwningPlayer(), PopupClass);
        if (ActivePopup) ActivePopup->AddToViewport(100);
    }
}

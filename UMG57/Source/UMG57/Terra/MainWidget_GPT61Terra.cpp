// Model: GPT-6.1 Terra; reasoning: Medium
#include "Terra/MainWidget_GPT61Terra.h"
#include "Terra/SliderPopupWidget_GPT61Terra.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
void UMainWidget_GPT61Terra::NativeConstruct()
{
    Super::NativeConstruct();
    SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    OpenButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OpenPopup);
    SliderOpenButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OpenSliderPopup);
}
void UMainWidget_GPT61Terra::NativeDestruct()
{
    OpenButton->OnClicked.RemoveDynamic(this, &ThisClass::OpenPopup);
    SliderOpenButton->OnClicked.RemoveDynamic(this, &ThisClass::OpenSliderPopup);
    if (ActivePopup) { ActivePopup->RemoveFromParent(); ActivePopup = nullptr; }
    Super::NativeDestruct();
}
void UMainWidget_GPT61Terra::OpenPopup() { ShowPopup(TEXT("/Game/UI/Terra/WBP_Popup_GPT61Terra.WBP_Popup_GPT61Terra_C"), false); }
void UMainWidget_GPT61Terra::OpenSliderPopup() { ShowPopup(TEXT("/Game/UI/Terra/WBP_SliderPopup_GPT61Terra.WBP_SliderPopup_GPT61Terra_C"), true); }
void UMainWidget_GPT61Terra::ShowPopup(const TCHAR* ClassPath, bool bSlider)
{
    UClass* PopupClass = LoadClass<UUserWidget>(nullptr, ClassPath);
    if (!PopupClass || !GetOwningPlayer()) return;
    if (ActivePopup && ActivePopup->IsInViewport())
    {
        if (ActivePopup->GetClass() == PopupClass) return;
        ActivePopup->RemoveFromParent();
    }
    ActivePopup = CreateWidget<UUserWidget>(GetOwningPlayer(), PopupClass);
    if (!ActivePopup) return;
    if (bSlider) if (auto* SliderPopup = Cast<USliderPopupWidget_GPT61Terra>(ActivePopup)) SliderPopup->SetMainWidget(this);
    ActivePopup->AddToViewport(100);
}

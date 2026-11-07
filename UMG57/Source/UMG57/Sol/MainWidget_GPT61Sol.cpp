// Model: GPT-6.1 Sol; reasoning: Medium
#include "Sol/MainWidget_GPT61Sol.h"
#include "Sol/SliderPopupWidget_GPT61Sol.h"
#include "Sol/PopupWidget_GPT61Sol.h"
#include "Components/Button.h"
void UMainWidget_GPT61Sol::NativeConstruct()
{
    Super::NativeConstruct();
    SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    if (OpenButton) OpenButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OpenPopup);
    if (SliderOpenButton) SliderOpenButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OpenSliderPopup);
}
void UMainWidget_GPT61Sol::OpenPopup() { OpenVariant(false); }
void UMainWidget_GPT61Sol::OpenSliderPopup() { OpenVariant(true); }
void UMainWidget_GPT61Sol::RememberWarmth(float Value) { Warmth = Value; }
void UMainWidget_GPT61Sol::OpenVariant(bool Slider)
{
    if (IsValid(ActivePopup) && ActivePopup->IsInViewport()
        && (ActivePopup->IsA<USliderPopupWidget_GPT61Sol>() == Slider)) return;
    if (!GetOwningPlayer()) return;
    const TCHAR* Path = Slider
        ? TEXT("/Game/UI/Sol/WBP_SliderPopup_GPT61Sol.WBP_SliderPopup_GPT61Sol_C")
        : TEXT("/Game/UI/Sol/WBP_Popup_GPT61Sol.WBP_Popup_GPT61Sol_C");
    const auto Class = LoadClass<UUserWidget>(nullptr, Path);
    if (!Class || (Slider ? !Class->IsChildOf(USliderPopupWidget_GPT61Sol::StaticClass())
        : !Class->IsChildOf(UPopupWidget_GPT61Sol::StaticClass()))) return;
    auto* NewPopup = CreateWidget<UUserWidget>(GetOwningPlayer(), Class);
    if (!NewPopup) return;
    if (auto* WarmthPopup = Cast<USliderPopupWidget_GPT61Sol>(NewPopup))
    {
        WarmthPopup->SetWarmth(Warmth);
        WarmthPopup->OnWarmthChanged.AddUObject(this, &ThisClass::RememberWarmth);
    }
    if (IsValid(ActivePopup)) ActivePopup->RemoveFromParent();
    ActivePopup = NewPopup;
    ActivePopup->AddToViewport(100);
}
void UMainWidget_GPT61Sol::NativeDestruct()
{
    if (OpenButton) OpenButton->OnClicked.RemoveDynamic(this, &ThisClass::OpenPopup);
    if (SliderOpenButton) SliderOpenButton->OnClicked.RemoveDynamic(this, &ThisClass::OpenSliderPopup);
    if (IsValid(ActivePopup)) ActivePopup->RemoveFromParent();
    ActivePopup = nullptr;
    Super::NativeDestruct();
}

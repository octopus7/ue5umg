// Model: GPT-6.1 Sol; reasoning: Medium
#include "Sol/SliderPopupWidget_GPT61Sol.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/ProgressBar.h"
void USliderPopupWidget_GPT61Sol::NativeConstruct()
{
    Super::NativeConstruct();
    SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    if (WarmthSlider) WarmthSlider->OnValueChanged.AddUniqueDynamic(this, &ThisClass::HandleValueChanged);
    if (CloseButton) CloseButton->OnClicked.AddUniqueDynamic(this, &ThisClass::ClosePopup);
    RefreshPreview();
}
void USliderPopupWidget_GPT61Sol::SetWarmth(float Value)
{
    const float Next = FMath::IsFinite(Value) ? FMath::Clamp(Value, 0.f, 1.f) : 0.68f;
    const bool Changed = !FMath::IsNearlyEqual(Warmth, Next);
    Warmth = Next;
    RefreshPreview();
    if (Changed) OnWarmthChanged.Broadcast(Warmth);
}
void USliderPopupWidget_GPT61Sol::HandleValueChanged(float Value)
{
    if (!bUpdating) SetWarmth(Value);
}
void USliderPopupWidget_GPT61Sol::RefreshPreview()
{
    TGuardValue<bool> Guard(bUpdating, true);
    if (WarmthSlider) WarmthSlider->SetValue(Warmth);
    if (WarmthFill) WarmthFill->SetPercent(Warmth);
    if (ValueText) ValueText->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), FMath::RoundToInt(Warmth * 100.f))));
    if (LightPreview) LightPreview->SetBrushColor(FMath::Lerp(
        FLinearColor(0.35f, 0.70f, 1.f), FLinearColor(1.f, 0.49f, 0.07f), Warmth));
}
void USliderPopupWidget_GPT61Sol::ClosePopup() { RemoveFromParent(); }
void USliderPopupWidget_GPT61Sol::NativeDestruct()
{
    if (WarmthSlider) WarmthSlider->OnValueChanged.RemoveDynamic(this, &ThisClass::HandleValueChanged);
    if (CloseButton) CloseButton->OnClicked.RemoveDynamic(this, &ThisClass::ClosePopup);
    OnWarmthChanged.Clear();
    Super::NativeDestruct();
}

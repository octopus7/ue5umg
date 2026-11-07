// Model: GPT-6.1 Terra; reasoning: Medium
#include "Terra/SliderPopupWidget_GPT61Terra.h"
#include "Terra/MainWidget_GPT61Terra.h"
#include "Components/Slider.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
void USliderPopupWidget_GPT61Terra::NativeConstruct()
{
    Super::NativeConstruct();
    const float Value = MainWidget ? MainWidget->GetRainIntensity() : 0.65f;
    IntensitySlider->SetValue(Value);
    IntensitySlider->OnValueChanged.AddUniqueDynamic(this, &ThisClass::OnIntensityChanged);
    OnIntensityChanged(Value);
}
void USliderPopupWidget_GPT61Terra::NativeDestruct()
{
    IntensitySlider->OnValueChanged.RemoveDynamic(this, &ThisClass::OnIntensityChanged);
    MainWidget = nullptr;
    Super::NativeDestruct();
}
void USliderPopupWidget_GPT61Terra::OnIntensityChanged(float Value)
{
    Value = FMath::Clamp(Value, 0.0f, 1.0f);
    if (MainWidget) MainWidget->SetRainIntensity(Value);
    IntensityFill->SetPercent(Value);
    ValueText->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), FMath::RoundToInt(Value * 100.0f))));
    RainPreview->SetBrushColor(FLinearColor(0.05f, 0.75f, 0.85f, 0.08f + 0.65f * Value));
}

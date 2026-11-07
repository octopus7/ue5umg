// Actual model: GPT-6 Astra; reasoning effort: Medium.
#include "Astra/VolumePopupWidget_GPT6Astra.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
void UVolumePopupWidget_GPT6Astra::NativeConstruct()
{
    Super::NativeConstruct();
    VolumeSlider->OnValueChanged.AddUniqueDynamic(this, &ThisClass::HandleValueChanged);
    CloseButton->OnClicked.AddUniqueDynamic(this, &ThisClass::ClosePopup);
    DoneButton->OnClicked.AddUniqueDynamic(this, &ThisClass::ClosePopup);
    DismissButton->OnClicked.AddUniqueDynamic(this, &ThisClass::ClosePopup);
    SetVolume(Volume);
}
void UVolumePopupWidget_GPT6Astra::SetVolume(float Value)
{
    Volume = FMath::Clamp(Value, 0.f, 1.f);
    if (VolumeSlider) VolumeSlider->SetValue(Volume);
    if (ValueText) ValueText->SetText(FText::FromString(FString::Printf(TEXT("%d%%"), FMath::RoundToInt(Volume * 100))));
    if (LevelPreview) LevelPreview->SetPercent(Volume);
}
void UVolumePopupWidget_GPT6Astra::HandleValueChanged(float Value)
{
    SetVolume(Value);
    OnVolumeChanged.ExecuteIfBound(Volume);
}
void UVolumePopupWidget_GPT6Astra::ClosePopup() { RemoveFromParent(); }
void UVolumePopupWidget_GPT6Astra::NativeDestruct()
{
    VolumeSlider->OnValueChanged.RemoveDynamic(this, &ThisClass::HandleValueChanged);
    CloseButton->OnClicked.RemoveDynamic(this, &ThisClass::ClosePopup);
    DoneButton->OnClicked.RemoveDynamic(this, &ThisClass::ClosePopup);
    DismissButton->OnClicked.RemoveDynamic(this, &ThisClass::ClosePopup);
    Super::NativeDestruct();
}

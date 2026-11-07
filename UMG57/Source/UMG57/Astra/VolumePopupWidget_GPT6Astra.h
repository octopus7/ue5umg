// Actual model: GPT-6 Astra; reasoning effort: Medium (verified session metadata).
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VolumePopupWidget_GPT6Astra.generated.h"
class UButton;
class USlider;
class UTextBlock;
class UProgressBar;
DECLARE_DELEGATE_OneParam(FAstraVolumeChanged, float);
UCLASS()
class UMG57_API UVolumePopupWidget_GPT6Astra : public UUserWidget
{
    GENERATED_BODY()
public:
    FAstraVolumeChanged OnVolumeChanged;
    void SetVolume(float Value);
    float GetVolume() const { return Volume; }
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
protected:
    UPROPERTY(meta=(BindWidget)) TObjectPtr<USlider> VolumeSlider;
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> ValueText;
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UProgressBar> LevelPreview;
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UButton> CloseButton;
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UButton> DoneButton;
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UButton> DismissButton;
    UFUNCTION() void HandleValueChanged(float Value);
    UFUNCTION() void ClosePopup();
    float Volume = .65f;
};

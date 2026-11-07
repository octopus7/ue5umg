// Model: GPT-6.1 Sol; reasoning: Medium
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SliderPopupWidget_GPT61Sol.generated.h"
class UButton;
class USlider;
class UTextBlock;
class UBorder;
class UProgressBar;
DECLARE_MULTICAST_DELEGATE_OneParam(FSolWarmthChanged, float);
UCLASS()
class UMG57_API USliderPopupWidget_GPT61Sol : public UUserWidget
{
    GENERATED_BODY()
public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
    UFUNCTION(BlueprintCallable, Category="Warmth") void SetWarmth(float Value);
    UFUNCTION(BlueprintPure, Category="Warmth") float GetWarmth() const { return Warmth; }
    FSolWarmthChanged OnWarmthChanged;
protected:
    UPROPERTY(meta=(BindWidget)) TObjectPtr<USlider> WarmthSlider;
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> ValueText;
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UBorder> LightPreview;
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UProgressBar> WarmthFill;
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UButton> CloseButton;
    UPROPERTY(Transient) float Warmth = 0.68f;
    bool bUpdating = false;
    UFUNCTION() void HandleValueChanged(float Value);
    UFUNCTION() void ClosePopup();
    void RefreshPreview();
};

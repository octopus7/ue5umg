// Model: GPT-6.1 Terra; reasoning: Medium
#pragma once
#include "CoreMinimal.h"
#include "Terra/PopupWidget_GPT61Terra.h"
#include "SliderPopupWidget_GPT61Terra.generated.h"
class USlider;
class UProgressBar;
class UTextBlock;
class UBorder;
class UMainWidget_GPT61Terra;
UCLASS()
class UMG57_API USliderPopupWidget_GPT61Terra : public UPopupWidget_GPT61Terra
{
    GENERATED_BODY()
public:
    void SetMainWidget(UMainWidget_GPT61Terra* InMain) { MainWidget = InMain; }
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
protected:
    UPROPERTY(meta=(BindWidget)) TObjectPtr<USlider> IntensitySlider;
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UProgressBar> IntensityFill;
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UTextBlock> ValueText;
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UBorder> RainPreview;
    UPROPERTY(Transient) TObjectPtr<UMainWidget_GPT61Terra> MainWidget;
    UFUNCTION() void OnIntensityChanged(float Value);
};

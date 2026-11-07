// Model: GPT-6.1 Terra; reasoning: Medium
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget_GPT61Terra.generated.h"
class UButton;
UCLASS()
class UMG57_API UMainWidget_GPT61Terra : public UUserWidget
{
    GENERATED_BODY()
public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
    UFUNCTION(BlueprintCallable, Category="Popup") void OpenPopup();
    UFUNCTION(BlueprintCallable, Category="Popup") void OpenSliderPopup();
    float GetRainIntensity() const { return RainIntensity; }
    void SetRainIntensity(float Value) { RainIntensity = FMath::Clamp(Value, 0.0f, 1.0f); }
protected:
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UButton> OpenButton;
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UButton> SliderOpenButton;
    UPROPERTY(Transient) TObjectPtr<UUserWidget> ActivePopup;
    UPROPERTY(Transient) float RainIntensity = 0.65f;
private:
    void ShowPopup(const TCHAR* ClassPath, bool bSlider);
};

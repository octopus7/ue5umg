// Actual model: GPT-6 Astra; reasoning effort: Medium (session metadata verified).
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget_GPT6Astra.generated.h"

class UButton;
class UPopupWidget_GPT6Astra;
class UVolumePopupWidget_GPT6Astra;

UCLASS()
class UMG57_API UMainWidget_GPT6Astra : public UUserWidget
{
    GENERATED_BODY()
public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
    void FocusOpener();
    UFUNCTION(BlueprintCallable) void OpenPopup();
    UFUNCTION(BlueprintCallable) void OpenSliderPopup();
    UFUNCTION() void OpenAllPopups();
    UFUNCTION() void OpenAllSliders();

protected:
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UButton> OpenButton;
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UButton> AllOpenButton;
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UButton> SliderOpenButton;
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UButton> AllSlidersButton;
    UPROPERTY(EditDefaultsOnly, Category="Popup") TSubclassOf<UPopupWidget_GPT6Astra> PopupClass;
    UPROPERTY(Transient) TObjectPtr<UPopupWidget_GPT6Astra> ActivePopup;
    UPROPERTY(Transient) TObjectPtr<UVolumePopupWidget_GPT6Astra> ActiveSlider;
    float SavedVolume = .65f;
    void SaveVolume(float Value) { SavedVolume = Value; }
    void PopupClosed();
};

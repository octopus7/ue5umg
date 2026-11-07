// Actual model: GPT-6 Astra; reasoning effort: Medium (session metadata verified).
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget_GPT6Astra.generated.h"

class UButton;
class UPopupWidget_GPT6Astra;

UCLASS()
class UMG57_API UMainWidget_GPT6Astra : public UUserWidget
{
    GENERATED_BODY()
public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
    void FocusOpener();

protected:
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UButton> OpenButton;
    UPROPERTY(EditDefaultsOnly, Category="Popup") TSubclassOf<UPopupWidget_GPT6Astra> PopupClass;
    UPROPERTY(Transient) TObjectPtr<UPopupWidget_GPT6Astra> ActivePopup;
    UFUNCTION() void OpenPopup();
    void PopupClosed();
};

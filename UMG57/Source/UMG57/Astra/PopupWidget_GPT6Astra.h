// Actual model: GPT-6 Astra; reasoning effort: Medium (session metadata verified).
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PopupWidget_GPT6Astra.generated.h"

class UButton;

UCLASS()
class UMG57_API UPopupWidget_GPT6Astra : public UUserWidget
{
    GENERATED_BODY()
public:
    FSimpleDelegate OnClosed;
    void ActivateModal();
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
    virtual FReply NativeOnPreviewKeyDown(const FGeometry&, const FKeyEvent&) override;
    virtual FReply NativeOnMouseButtonDown(const FGeometry&, const FPointerEvent&) override;
    virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry&, const FPointerEvent&) override;
    virtual void NativeOnFocusChanging(const FWeakWidgetPath&, const FWidgetPath&, const FFocusEvent&) override;

protected:
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UButton> CloseButton;
    UFUNCTION() void ClosePopup();
    bool bModalActive = false;
};

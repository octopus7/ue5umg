// Actual model: GPT-6 Astra; reasoning effort: Medium.
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
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
protected:
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UButton> CloseButton;
    UPROPERTY(meta=(BindWidget)) TObjectPtr<UButton> DismissButton;
    UFUNCTION() void ClosePopup();
};

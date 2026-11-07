// Model: GPT-6.1 Sol; reasoning: Medium
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PopupWidget_GPT61Sol.generated.h"
class UButton;
UCLASS() class UMG57_API UPopupWidget_GPT61Sol : public UUserWidget { GENERATED_BODY() public: virtual void NativeConstruct() override; virtual void NativeTick(const FGeometry&,float) override; protected: UPROPERTY(meta=(BindWidget)) TObjectPtr<UButton> CloseButton; UFUNCTION() void ClosePopup(); };
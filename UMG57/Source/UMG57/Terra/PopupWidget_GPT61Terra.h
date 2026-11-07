// Model: GPT-6.1 Terra; reasoning: Medium
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PopupWidget_GPT61Terra.generated.h"
class UButton;
UCLASS() class UMG57_API UPopupWidget_GPT61Terra : public UUserWidget { GENERATED_BODY() public: virtual void NativeConstruct() override; virtual void NativeTick(const FGeometry&,float) override; protected: UPROPERTY(meta=(BindWidget)) TObjectPtr<UButton> CloseButton; UFUNCTION() void ClosePopup(); };
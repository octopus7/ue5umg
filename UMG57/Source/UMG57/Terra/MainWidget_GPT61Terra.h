// Model: GPT-6.1 Terra; reasoning: Medium
#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget_GPT61Terra.generated.h"
class UButton;
UCLASS() class UMG57_API UMainWidget_GPT61Terra:public UUserWidget { GENERATED_BODY() public: virtual void NativeConstruct() override; protected: UPROPERTY(meta=(BindWidget)) TObjectPtr<UButton> OpenButton; UFUNCTION() void OpenPopup(); };
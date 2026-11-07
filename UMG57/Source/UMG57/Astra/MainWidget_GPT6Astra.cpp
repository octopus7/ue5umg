// Actual model: GPT-6 Astra; reasoning effort: Medium.
#include "Astra/MainWidget_GPT6Astra.h"
#include "Astra/PopupWidget_GPT6Astra.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"

void UMainWidget_GPT6Astra::NativeConstruct()
{
    Super::NativeConstruct();
    OpenButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OpenPopup);
}

void UMainWidget_GPT6Astra::FocusOpener()
{
    if (auto* Player = GetOwningPlayer())
    {
        FInputModeUIOnly Mode;
        Mode.SetWidgetToFocus(OpenButton->TakeWidget());
        Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        Player->SetInputMode(Mode);
        OpenButton->SetUserFocus(Player);
    }
}

void UMainWidget_GPT6Astra::OpenPopup()
{
    if (ActivePopup || !PopupClass || !GetOwningPlayer()) return;
    ActivePopup = CreateWidget<UPopupWidget_GPT6Astra>(GetOwningPlayer(), PopupClass);
    if (!ActivePopup) return;
    ActivePopup->OnClosed.BindUObject(this, &ThisClass::PopupClosed);
    OpenButton->SetIsEnabled(false);
    ActivePopup->AddToViewport(100);
    ActivePopup->ActivateModal();
}

void UMainWidget_GPT6Astra::PopupClosed()
{
    ActivePopup = nullptr;
    OpenButton->SetIsEnabled(true);
    FocusOpener();
}

void UMainWidget_GPT6Astra::NativeDestruct()
{
    if (ActivePopup)
    {
        ActivePopup->OnClosed.Unbind();
        ActivePopup->RemoveFromParent();
        ActivePopup = nullptr;
    }
    OpenButton->OnClicked.RemoveDynamic(this, &ThisClass::OpenPopup);
    Super::NativeDestruct();
}

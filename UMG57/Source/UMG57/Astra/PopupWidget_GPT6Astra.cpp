// Actual model: GPT-6 Astra; reasoning effort: Medium.
#include "Astra/PopupWidget_GPT6Astra.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "Framework/Application/SlateApplication.h"
#include "Layout/WidgetPath.h"
#include "InputCoreTypes.h"

void UPopupWidget_GPT6Astra::NativeConstruct()
{
    Super::NativeConstruct();
    SetIsFocusable(true);
    CloseButton->OnClicked.AddUniqueDynamic(this, &ThisClass::ClosePopup);
}

void UPopupWidget_GPT6Astra::ActivateModal()
{
    if (auto* Player = GetOwningPlayer())
    {
        bModalActive = true;
        FInputModeUIOnly Mode;
        Mode.SetWidgetToFocus(CloseButton->TakeWidget());
        Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        Player->SetInputMode(Mode);
        CloseButton->SetUserFocus(Player);
    }
}

FReply UPopupWidget_GPT6Astra::NativeOnPreviewKeyDown(const FGeometry& Geometry, const FKeyEvent& Event)
{
    // With one action in this modal, forward and reverse traversal both stay here.
    if (Event.GetKey() == EKeys::Tab)
        return FReply::Handled().SetUserFocus(CloseButton->TakeWidget(), EFocusCause::Navigation);
    return Super::NativeOnPreviewKeyDown(Geometry, Event);
}

FReply UPopupWidget_GPT6Astra::NativeOnMouseButtonDown(const FGeometry&, const FPointerEvent&)
{
    // The WBP backdrop fills the viewport. Consume clicks which the Close button did not handle.
    return FReply::Handled().SetUserFocus(CloseButton->TakeWidget(), EFocusCause::Mouse);
}

FReply UPopupWidget_GPT6Astra::NativeOnMouseButtonDoubleClick(const FGeometry& Geometry, const FPointerEvent& Event)
{
    return NativeOnMouseButtonDown(Geometry, Event);
}

void UPopupWidget_GPT6Astra::NativeOnFocusChanging(const FWeakWidgetPath& OldPath, const FWidgetPath& NewPath, const FFocusEvent& Event)
{
    Super::NativeOnFocusChanging(OldPath, NewPath, Event);
    // Cancel an in-window escape before Slate commits it, rather than repair it next tick.
    // Other windows (including the editor and OS application switching) remain usable.
    if (bModalActive && NewPath.IsValid() && OldPath.Window.Pin() == NewPath.GetWindow()
        && !NewPath.ContainsWidget(&TakeWidget().Get()))
    {
        // FSlateUser's version counter is private API. A nested, real focus change
        // within the modal invalidates the pending escape through supported Slate APIs.
        auto& Slate = FSlateApplication::Get();
        Slate.SetUserFocus(Event.GetUser(), TakeWidget(), EFocusCause::SetDirectly);
        Slate.SetUserFocus(Event.GetUser(), CloseButton->TakeWidget(), EFocusCause::SetDirectly);
    }
}

void UPopupWidget_GPT6Astra::ClosePopup()
{
    bModalActive = false;
    RemoveFromParent();
    OnClosed.ExecuteIfBound();
}

void UPopupWidget_GPT6Astra::NativeDestruct()
{
    bModalActive = false;
    CloseButton->OnClicked.RemoveDynamic(this, &ThisClass::ClosePopup);
    Super::NativeDestruct();
}


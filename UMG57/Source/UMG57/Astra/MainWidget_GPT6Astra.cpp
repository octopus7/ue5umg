// Actual model: GPT-6 Astra; reasoning effort: Medium.
#include "Astra/MainWidget_GPT6Astra.h"
#include "Astra/PopupWidget_GPT6Astra.h"
#include "Astra/VolumePopupWidget_GPT6Astra.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Sol/MainWidget_GPT61Sol.h"
#include "Terra/MainWidget_GPT61Terra.h"

void UMainWidget_GPT6Astra::NativeConstruct()
{
    Super::NativeConstruct();
    OpenButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OpenPopup);
    AllOpenButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OpenAllPopups);
    SliderOpenButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OpenSliderPopup);
    AllSlidersButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OpenAllSliders);
}

void UMainWidget_GPT6Astra::FocusOpener()
{
    if (auto* Player = GetOwningPlayer())
    {
        FInputModeGameAndUI Mode;
        Mode.SetWidgetToFocus(OpenButton->TakeWidget());
        Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        Player->SetInputMode(Mode);
        OpenButton->SetUserFocus(Player);
    }
}

void UMainWidget_GPT6Astra::OpenPopup()
{
    if ((ActivePopup && ActivePopup->IsInViewport()) || !PopupClass || !GetOwningPlayer()) return;
    if (ActiveSlider) { ActiveSlider->OnVolumeChanged.Unbind(); ActiveSlider->RemoveFromParent(); ActiveSlider = nullptr; }
    ActivePopup = CreateWidget<UPopupWidget_GPT6Astra>(GetOwningPlayer(), PopupClass);
    if (!ActivePopup) return;
    ActivePopup->OnClosed.BindUObject(this, &ThisClass::PopupClosed);
    ActivePopup->AddToViewport(100);
}

void UMainWidget_GPT6Astra::OpenSliderPopup()
{
    if ((ActiveSlider && ActiveSlider->IsInViewport()) || !GetOwningPlayer()) return;
    auto* Class = LoadClass<UVolumePopupWidget_GPT6Astra>(nullptr, TEXT("/Game/UI/Astra/WBP_VolumePopup_GPT6Astra.WBP_VolumePopup_GPT6Astra_C"));
    if (!Class) return;
    if (ActivePopup) { ActivePopup->OnClosed.Unbind(); ActivePopup->RemoveFromParent(); ActivePopup = nullptr; }
    ActiveSlider = CreateWidget<UVolumePopupWidget_GPT6Astra>(GetOwningPlayer(), Class);
    if (!ActiveSlider) return;
    ActiveSlider->SetVolume(SavedVolume);
    ActiveSlider->OnVolumeChanged.BindUObject(this, &ThisClass::SaveVolume);
    ActiveSlider->AddToViewport(100);
}

void UMainWidget_GPT6Astra::OpenAllSliders()
{
    OpenSliderPopup();
    TArray<UUserWidget*> Widgets;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, Widgets, UMainWidget_GPT61Sol::StaticClass(), true);
    for (auto* Widget : Widgets) CastChecked<UMainWidget_GPT61Sol>(Widget)->OpenSliderPopup();
    Widgets.Reset();
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, Widgets, UMainWidget_GPT61Terra::StaticClass(), true);
    for (auto* Widget : Widgets) CastChecked<UMainWidget_GPT61Terra>(Widget)->OpenSliderPopup();
}

void UMainWidget_GPT6Astra::OpenAllPopups()
{
    OpenPopup();
    TArray<UUserWidget*> Widgets;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, Widgets, UMainWidget_GPT61Sol::StaticClass(), true);
    for (auto* Widget : Widgets) CastChecked<UMainWidget_GPT61Sol>(Widget)->OpenPopup();
    Widgets.Reset();
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, Widgets, UMainWidget_GPT61Terra::StaticClass(), true);
    for (auto* Widget : Widgets) CastChecked<UMainWidget_GPT61Terra>(Widget)->OpenPopup();
}

void UMainWidget_GPT6Astra::PopupClosed()
{
    ActivePopup = nullptr;
}

void UMainWidget_GPT6Astra::NativeDestruct()
{
    if (ActiveSlider) { ActiveSlider->OnVolumeChanged.Unbind(); ActiveSlider->RemoveFromParent(); ActiveSlider = nullptr; }
    if (ActivePopup)
    {
        ActivePopup->OnClosed.Unbind();
        ActivePopup->RemoveFromParent();
        ActivePopup = nullptr;
    }
    OpenButton->OnClicked.RemoveDynamic(this, &ThisClass::OpenPopup);
    AllOpenButton->OnClicked.RemoveDynamic(this, &ThisClass::OpenAllPopups);
    SliderOpenButton->OnClicked.RemoveDynamic(this, &ThisClass::OpenSliderPopup);
    AllSlidersButton->OnClicked.RemoveDynamic(this, &ThisClass::OpenAllSliders);
    Super::NativeDestruct();
}

#include "Misc/AutomationTest.h"
#include "WidgetBlueprint.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"

#if WITH_DEV_AUTOMATION_TESTS
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModelButtonsTest, "UMG57.Models.LabeledButtons",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FModelButtonsTest::RunTest(const FString& Parameters)
{
    const TCHAR* Folders[] = {TEXT("Astra"), TEXT("Sol"), TEXT("Terra")};
    const TCHAR* Suffixes[] = {TEXT("GPT6Astra"), TEXT("GPT61Sol"), TEXT("GPT61Terra")};
    const TCHAR* Labels[] = {TEXT("Open GPT-6 Astra Popup"), TEXT("Open GPT-6.1 Sol Popup"), TEXT("Open GPT-6.1 Terra Popup")};
    for (int32 Index = 0; Index < 3; ++Index)
    {
        const FString Path = FString::Printf(TEXT("/Game/UI/%s/WBP_Main_%s"), Folders[Index], Suffixes[Index]);
        auto* BP = LoadObject<UWidgetBlueprint>(nullptr, *Path);
        if (!TestNotNull(*Path, BP)) continue;
        auto* Label = Cast<UTextBlock>(BP->WidgetTree->FindWidget(TEXT("ButtonLabel")));
        if (TestNotNull(TEXT("Button label exists"), Label)) TestEqual(TEXT("Explicit model label"), Label->GetText().ToString(), FString(Labels[Index]));
        auto* Button = BP->WidgetTree->FindWidget(TEXT("OpenButton"));
        auto* Slot = Button ? Cast<UCanvasPanelSlot>(Button->Slot) : nullptr;
        if (TestNotNull(TEXT("Saved button placement"), Slot)) TestEqual(TEXT("Distinct row"), Slot->GetPosition().Y, (Index - 1) * 90.0);
    }
    return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAstraAssetsTest, "UMG57.Astra.SavedWidgetTrees",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FAstraAssetsTest::RunTest(const FString& Parameters)
{
    auto* Main = LoadObject<UWidgetBlueprint>(nullptr, TEXT("/Game/UI/Astra/WBP_Main_GPT6Astra.WBP_Main_GPT6Astra"));
    auto* Popup = LoadObject<UWidgetBlueprint>(nullptr, TEXT("/Game/UI/Astra/WBP_Popup_GPT6Astra.WBP_Popup_GPT6Astra"));
    if (!TestNotNull(TEXT("Saved main blueprint"), Main) || !TestNotNull(TEXT("Saved popup blueprint"), Popup)) return false;
    TestNotNull(TEXT("Editable main root"), Main->WidgetTree->RootWidget.Get());
    TestNotNull(TEXT("Editable popup root"), Popup->WidgetTree->RootWidget.Get());
    TestNotNull(TEXT("Bound opener"), Cast<UButton>(Main->WidgetTree->FindWidget(TEXT("OpenButton"))));
    TestNotNull(TEXT("Bound closer"), Cast<UButton>(Popup->WidgetTree->FindWidget(TEXT("CloseButton"))));
    auto* Message = Cast<UTextBlock>(Popup->WidgetTree->FindWidget(TEXT("Message")));
    if (TestNotNull(TEXT("Saved message"), Message)) TestFalse(TEXT("Nonempty message"), Message->GetText().IsEmpty());
    TestNotNull(TEXT("Full-screen backdrop"), Popup->WidgetTree->FindWidget(TEXT("Backdrop")));
    TestEqual(TEXT("Main parent"), Main->ParentClass->GetName(), FString(TEXT("MainWidget_GPT6Astra")));
    TestEqual(TEXT("Popup parent"), Popup->ParentClass->GetName(), FString(TEXT("PopupWidget_GPT6Astra")));
    return true;
}
#endif


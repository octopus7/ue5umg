#include "Misc/AutomationTest.h"
#include "WidgetBlueprint.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/BackgroundBlur.h"
#include "Components/Border.h"

#if WITH_DEV_AUTOMATION_TESTS
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModelPanelsTest, "UMG57.Models.NonModalPanels",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FModelPanelsTest::RunTest(const FString& Parameters)
{
    const TCHAR* Folders[] = {TEXT("Astra"), TEXT("Sol"), TEXT("Terra")};
    const TCHAR* Suffixes[] = {TEXT("GPT6Astra"), TEXT("GPT61Sol"), TEXT("GPT61Terra")};
    for (int32 Index = 0; Index < 3; ++Index)
    {
        const FString MainPath = FString::Printf(TEXT("/Game/UI/%s/WBP_Main_%s"), Folders[Index], Suffixes[Index]);
        const FString PopupPath = FString::Printf(TEXT("/Game/UI/%s/WBP_Popup_%s"), Folders[Index], Suffixes[Index]);
        auto* Main = LoadObject<UWidgetBlueprint>(nullptr, *MainPath);
        auto* Popup = LoadObject<UWidgetBlueprint>(nullptr, *PopupPath);
        if (!TestNotNull(*MainPath, Main) || !TestNotNull(*PopupPath, Popup)) continue;
        auto* Open = Cast<UButton>(Main->WidgetTree->FindWidget(TEXT("OpenButton")));
        if (TestNotNull(TEXT("Open button"), Open))
        {
            auto* Slot = Cast<UCanvasPanelSlot>(Open->Slot);
            if (TestNotNull(TEXT("Opener canvas slot"), Slot))
            {
                TestTrue(TEXT("Opener in model column"), FMath::IsNearlyEqual(Slot->GetAnchors().Minimum.X, (Index+.5)/3, .001));
                TestEqual(TEXT("Opener row"), Slot->GetPosition().Y, 80.0);
            }
        }
        if (Index == 0) TestNotNull(TEXT("Open all button"), Main->WidgetTree->FindWidget(TEXT("AllOpenButton")));
        TestNotNull(TEXT("Title"), Cast<UTextBlock>(Popup->WidgetTree->FindWidget(TEXT("Title"))));
        TestNotNull(TEXT("Body"), Cast<UTextBlock>(Popup->WidgetTree->FindWidget(TEXT("Message"))));
        TestNotNull(TEXT("Close button"), Cast<UButton>(Popup->WidgetTree->FindWidget(TEXT("CloseButton"))));
        TestNull(TEXT("No fullscreen backdrop"), Popup->WidgetTree->FindWidget(TEXT("Backdrop")));
        TestEqual(TEXT("Root allows outside clicks"), Popup->WidgetTree->RootWidget->GetVisibility(), ESlateVisibility::SelfHitTestInvisible);
        TArray<UWidget*> Widgets; Popup->WidgetTree->GetAllWidgets(Widgets);
        UBackgroundBlur* Blur = nullptr;
        UBorder* Surface = nullptr;
        for (auto* Widget : Widgets)
        {
            TestTrue(TEXT("Saved widget GUID"), Popup->WidgetVariableNameToGuidMap.Contains(Widget->GetFName()));
            if (auto* B = Cast<UBackgroundBlur>(Widget)) Blur=B;
            if (auto* B = Cast<UBorder>(Widget)) Surface=B;
        }
        if (TestNotNull(TEXT("Background blur"), Blur))
        {
            TestEqual(TEXT("Blur strength"), Blur->GetBlurStrength(), 8.f);
            TestTrue(TEXT("Rounded blur"), Blur->GetCornerRadius().Equals(FVector4(20,20,20,20)));
            auto* Slot = Cast<UCanvasPanelSlot>(Blur->Slot);
            if (TestNotNull(TEXT("Panel canvas slot"), Slot))
            {
                TestTrue(TEXT("Panel left boundary"), FMath::IsNearlyEqual(Slot->GetAnchors().Minimum.X, Index/3.0, .001));
                TestTrue(TEXT("Panel right boundary"), FMath::IsNearlyEqual(Slot->GetAnchors().Maximum.X, (Index+1)/3.0, .001));
            }
        }
        if (TestNotNull(TEXT("Translucent surface"), Surface))
        {
            const auto& Brush = Surface->Background;
            TestEqual(TEXT("Rounded box"), Brush.DrawAs.GetValue(), ESlateBrushDrawType::RoundedBox);
            TestTrue(TEXT("Fixed corner radius"), Brush.OutlineSettings.CornerRadii.Equals(FVector4(20,20,20,20)));
            TestTrue(TEXT("Translucent tint"), Brush.TintColor.GetSpecifiedColor().A > 0 && Brush.TintColor.GetSpecifiedColor().A < 1);
        }
    }
    return true;
}
#endif


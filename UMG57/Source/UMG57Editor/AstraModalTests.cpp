// Actual model: GPT-6 Astra; reasoning effort: Medium.
// Shared acceptance tests for editable, nonmodal concept implementations.
#include "Misc/AutomationTest.h"
#include "WidgetBlueprint.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "Components/ProgressBar.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/BackgroundBlur.h"
#include "Components/Border.h"
#include "Components/OverlaySlot.h"
#if WITH_DEV_AUTOMATION_TESTS
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FModelPanelsTest, "UMG57.Models.NonModalPanels",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FModelPanelsTest::RunTest(const FString& Parameters)
{
    const TCHAR* Folders[]={TEXT("Astra"),TEXT("Sol"),TEXT("Terra")};
    const TCHAR* Suffixes[]={TEXT("GPT6Astra"),TEXT("GPT61Sol"),TEXT("GPT61Terra")};
    const TCHAR* SliderNames[]={TEXT("VolumeSlider"),TEXT("WarmthSlider"),TEXT("IntensitySlider")};
    const float Defaults[]={.65f,.68f,.65f};
    for (int32 I=0;I<3;++I)
    {
        auto* Main=LoadObject<UWidgetBlueprint>(nullptr,*FString::Printf(TEXT("/Game/UI/%s/WBP_Main_%s"),Folders[I],Suffixes[I]));
        if (!TestNotNull(TEXT("Saved main blueprint"),Main)) continue;
        for (int32 Row=0;Row<2;++Row)
        {
            auto* Open=Cast<UButton>(Main->WidgetTree->FindWidget(Row?TEXT("SliderOpenButton"):TEXT("OpenButton")));
            if (!TestNotNull(TEXT("Variant opener"),Open)) continue;
            auto* Slot=Cast<UCanvasPanelSlot>(Open->Slot);
            if (!TestNotNull(TEXT("Opener canvas slot"),Slot)) continue;
            TestTrue(TEXT("Model column"),FMath::IsNearlyEqual(Slot->GetAnchors().Minimum.X,(I+.5)/3,.001));
            TestEqual(TEXT("Opener row"),Slot->GetPosition().Y,Row?125.0:60.0);
        }
        if (I==0)
        {
            TestNotNull(TEXT("Open all basic"),Main->WidgetTree->FindWidget(TEXT("AllOpenButton")));
            TestNotNull(TEXT("Open all sliders"),Main->WidgetTree->FindWidget(TEXT("AllSlidersButton")));
        }
        for (int32 Variant=0;Variant<2;++Variant)
        {
            const TCHAR* Prefix=Variant?(I==0?TEXT("VolumePopup"):TEXT("SliderPopup")):TEXT("Popup");
            const FString Path=FString::Printf(TEXT("/Game/UI/%s/WBP_%s_%s"),Folders[I],Prefix,Suffixes[I]);
            auto* BP=LoadObject<UWidgetBlueprint>(nullptr,*Path);
            if (!TestNotNull(*Path,BP)) continue;
            TestTrue(TEXT("Compiled blueprint"),BP->Status!=BS_Error);
            auto* T=BP->WidgetTree.Get();
            TestNotNull(TEXT("Editable title"),Cast<UTextBlock>(T->FindWidget(TEXT("Title"))));
            TestNotNull(TEXT("Editable body"),Cast<UTextBlock>(T->FindWidget(TEXT("Message"))));
            TestNotNull(TEXT("Close button"),Cast<UButton>(T->FindWidget(TEXT("CloseButton"))));
            TestEqual(TEXT("Outside input remains available"),T->RootWidget->GetVisibility(),ESlateVisibility::SelfHitTestInvisible);
            TestNull(TEXT("No fullscreen blocker"),T->FindWidget(TEXT("Backdrop")));
            TArray<UWidget*> Widgets; T->GetAllWidgets(Widgets); UBackgroundBlur* Blur=nullptr; bool Rounded=false;
            for (auto* W:Widgets)
            {
                TestTrue(TEXT("Serialized widget GUID"),BP->WidgetVariableNameToGuidMap.Contains(W->GetFName()));
                if (auto* B=Cast<UBackgroundBlur>(W)) Blur=B;
                if (auto* B=Cast<UBorder>(W)) Rounded|=B->Background.DrawAs==ESlateBrushDrawType::RoundedBox;
            }
            TestTrue(TEXT("Rounded native surface"),Rounded);
            if (TestNotNull(TEXT("Blur present"),Blur))
            {
                TestTrue(TEXT("Blur enabled"),Blur->GetBlurStrength()>0);
                auto* Slot=Cast<UCanvasPanelSlot>(Blur->Slot);
                if (TestNotNull(TEXT("Bounded popup"),Slot))
                {
                    TestTrue(TEXT("Left edge"),FMath::IsNearlyEqual(Slot->GetAnchors().Minimum.X,I/3.0,.001));
                    TestTrue(TEXT("Right edge"),FMath::IsNearlyEqual(Slot->GetAnchors().Maximum.X,(I+1)/3.0,.001));
                    TestTrue(TEXT("Opener space reserved"),Slot->GetAnchors().Minimum.Y>=.29);
                }
            }
            if (Variant)
            {
                auto* Slider=Cast<USlider>(T->FindWidget(SliderNames[I]));
                TestNotNull(TEXT("Live value label"),Cast<UTextBlock>(T->FindWidget(TEXT("ValueText"))));
                if (TestNotNull(TEXT("Native horizontal slider"),Slider))
                {
                    TestTrue(TEXT("Design initial value"),FMath::IsNearlyEqual(Slider->GetValue(),Defaults[I],.001f));
                    TestEqual(TEXT("Horizontal"),Slider->GetOrientation(),Orient_Horizontal);
                    TestTrue(TEXT("Keyboard focusable"),Slider->IsFocusable);
                    TestTrue(TEXT("Fine adjustment"),Slider->GetStepSize()>0 && Slider->GetStepSize()<=.05f);
                    if (auto* S=Cast<UOverlaySlot>(Slider->Slot))
                        TestEqual(TEXT("Slider fills its track width"),S->GetHorizontalAlignment(),HAlign_Fill);
                }
            }
        }
    }
    return true;
}
#endif

// Model: GPT-6.1 Sol; reasoning: Medium
#if WITH_DEV_AUTOMATION_TESTS
#include "Misc/AutomationTest.h"
#include "Sol/SliderPopupWidget_GPT61Sol.h"
#include <limits>
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSolWarmthStateTest, "UMG57.Sol.Concepts.WarmthState",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
bool FSolWarmthStateTest::RunTest(const FString& Parameters)
{
    auto* Popup = NewObject<USliderPopupWidget_GPT61Sol>();
    TestEqual(TEXT("Initial warmth"), Popup->GetWarmth(), 0.68f);
    int32 Changes = 0;
    float Observed = -1.f;
    Popup->OnWarmthChanged.AddLambda([&](float Value) { ++Changes; Observed = Value; });
    Popup->SetWarmth(2.f);
    TestEqual(TEXT("Upper clamp"), Popup->GetWarmth(), 1.f);
    TestEqual(TEXT("Owner receives stored value"), Observed, 1.f);
    Popup->SetWarmth(1.f);
    TestEqual(TEXT("No duplicate notification"), Changes, 1);
    Popup->SetWarmth(-1.f);
    TestEqual(TEXT("Lower clamp"), Popup->GetWarmth(), 0.f);
    Popup->SetWarmth(std::numeric_limits<float>::quiet_NaN());
    TestEqual(TEXT("Invalid value recovers default"), Popup->GetWarmth(), 0.68f);
    auto* Reopened = NewObject<USliderPopupWidget_GPT61Sol>();
    Popup->SetWarmth(0.42f);
    Reopened->SetWarmth(Observed);
    TestEqual(TEXT("Owner value can initialize a reopened popup"), Reopened->GetWarmth(), 0.42f);
    Popup->OnWarmthChanged.Clear();
    return true;
}
#endif

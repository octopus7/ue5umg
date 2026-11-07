// Model: GPT-6.1 Sol; reasoning: Medium
// Editor-only authoring: all presentation elements are serialized in WBP trees.
#include "GenerateSolPanelAssets.h"
#include "WidgetBlueprint.h"
#include "WidgetBlueprintFactory.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Components/BackgroundBlur.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/SizeBox.h"
#include "Components/Spacer.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/Slider.h"
#include "Components/ProgressBar.h"
#include "Sol/MainWidget_GPT61Sol.h"
#include "Sol/PopupWidget_GPT61Sol.h"
#include "Sol/SliderPopupWidget_GPT61Sol.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"
#include "Misc/PackageName.h"

namespace SolPanelAssets
{
const FLinearColor Cobalt(0.031f, 0.105f, 1.f);
const FLinearColor Amber(1.f, 0.40f, 0.015f);
const FLinearColor Ivory(0.92f, 0.89f, 0.82f);
const FLinearColor Ink(0.015f, 0.025f, 0.022f);
FSlateBrush Rounded(FLinearColor Color, float Radius)
{
    FSlateBrush B;
    B.DrawAs = ESlateBrushDrawType::RoundedBox;
    B.TintColor = FSlateColor(Color);
    B.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
    B.OutlineSettings.CornerRadii = FVector4(Radius, Radius, Radius, Radius);
    return B;
}
UTextBlock* Text(UWidgetTree* Tree, const TCHAR* Name, const TCHAR* Value, int32 Size, FLinearColor Color, bool Serif = false)
{
    auto* W = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), Name);
    W->SetText(FText::FromString(Value)); W->SetAutoWrapText(true);
    auto Font = W->GetFont(); Font.Size = Size;
    // Use the embedded default typeface so serialized WBP text renders in game.
    Font.TypefaceFontName = Serif ? FName(TEXT("Light")) : FName(TEXT("Regular"));
    W->SetFont(Font); W->SetColorAndOpacity(FSlateColor(Color));
    return W;
}
void Add(UVerticalBox* Box, UWidget* W, float Bottom = 10.f, bool Fill = false)
{
    auto* S = Box->AddChildToVerticalBox(W); S->SetPadding(FMargin(0, 0, 0, Bottom));
    if (Fill) S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
}
UBorder* Line(UWidgetTree* T, const TCHAR* Name, FLinearColor Color)
{
    auto* B = T->ConstructWidget<UBorder>(UBorder::StaticClass(), Name);
    B->SetBrushColor(Color); B->SetPadding(FMargin(0, 1)); return B;
}
UButton* Button(UWidgetTree* T, const TCHAR* Name, const TCHAR* Label, FLinearColor Color, FLinearColor Foreground)
{
    auto* B = T->ConstructWidget<UButton>(UButton::StaticClass(), Name);
    auto Style = B->GetStyle();
    Style.Normal = Rounded(Color, 10); Style.Hovered = Rounded(Color * 0.85f, 10);
    Style.Pressed = Rounded(Color * 0.65f, 10); B->SetStyle(Style);
    auto* LabelWidget = Text(T, *(FString(Name) + TEXT("Label")), Label, 18, Foreground);
    LabelWidget->SetJustification(ETextJustify::Center); B->AddChild(LabelWidget); return B;
}
bool Generate(const TCHAR* Name, UClass* Parent, int32 Kind)
{
    const FString Path = FString(TEXT("/Game/UI/Sol/")) + Name;
    auto* BP = LoadObject<UWidgetBlueprint>(nullptr, *Path);
    if (BP && BP->ParentClass != Parent)
    {
        UE_LOG(LogTemp, Error, TEXT("Sol asset parent mismatch: %s"), *Path); return false;
    }
    if (!BP)
    {
        auto* P = CreatePackage(*Path); auto* F = NewObject<UWidgetBlueprintFactory>(); F->ParentClass = Parent;
        BP = Cast<UWidgetBlueprint>(F->FactoryCreateNew(UWidgetBlueprint::StaticClass(), P, FName(Name), RF_Public | RF_Standalone, nullptr, GWarn));
        if (!BP) return false;
        FAssetRegistryModule::AssetCreated(BP);
    }
    BP->Modify(); BP->WidgetTree = NewObject<UWidgetTree>(BP, NAME_None, RF_Transactional);
    UWidgetTree* Tree = BP->WidgetTree.Get();
    auto* Root = Tree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("Root"));
    Root->SetVisibility(ESlateVisibility::SelfHitTestInvisible); Tree->RootWidget = Root;
    if (Kind == 0)
    {
        for (int32 I = 0; I < 2; ++I)
        {
            auto* B = Button(Tree, I ? TEXT("SliderOpenButton") : TEXT("OpenButton"),
                I ? TEXT("Sol / Warmth") : TEXT("Sol / Note"), I ? Amber : Cobalt, I ? Ink : FLinearColor::White);
            if (auto* Label = Cast<UTextBlock>(B->GetContent())) Label->SetAutoWrapText(false);
            auto* S = Root->AddChildToCanvas(B); S->SetAnchors(FAnchors(0.5f, 0));
            S->SetAlignment(FVector2D(0.5f, 0.5f)); S->SetPosition(FVector2D(0, I ? 125 : 60)); S->SetSize(FVector2D(300, 52));
        }
    }
    else
    {
        const bool Slider = Kind == 2;
        const FLinearColor Foreground = Slider ? Ivory : Ink;
        auto* Card = Tree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Card"));
        Card->SetBrush(Rounded(Slider ? FLinearColor(0.002f, 0.038f, 0.034f, 0.90f) : FLinearColor(0.92f, 0.90f, 0.84f, 0.94f), 20));
        Card->SetPadding(FMargin(22, 20));
        auto* Blur = Tree->ConstructWidget<UBackgroundBlur>(UBackgroundBlur::StaticClass(), TEXT("CardBlur"));
        Blur->SetBlurStrength(Slider ? 8.f : 3.f);
        Blur->SetCornerRadius(FVector4(20, 20, 20, 20));
        Blur->SetApplyAlphaToBlur(false);
        Blur->AddChild(Card);
        auto* CardSlot = Root->AddChildToCanvas(Blur);
        CardSlot->SetAnchors(FAnchors(1.f/3.f, 0.30f, 2.f/3.f, 0.94f)); CardSlot->SetOffsets(FMargin(16, 0, 16, 0));
        auto* Content = Tree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("Content")); Card->AddChild(Content);
        Add(Content, Line(Tree, TEXT("AccentRule"), Slider ? Amber : Cobalt), 10);
        Add(Content, Text(Tree, TEXT("Eyebrow"), Slider ? TEXT("ROOM LIGHTING") : TEXT("ALL SET   /   \u2713"), 12, Slider ? Ivory : Cobalt), 8);
        Add(Content, Text(Tree, TEXT("Title"), Slider ? TEXT("Find your\nevening glow.") : TEXT("A little clarity.\nA fresh start."), Slider ? 25 : 29, Foreground, !Slider), 10);
        Add(Content, Text(Tree, TEXT("Message"), Slider ? TEXT("Adjust the warmth to make your space feel just right.") : TEXT("Your workspace is ready. Everything you need is now in place."), 15, Foreground), 12, !Slider);
        if (Slider)
        {
            auto* Row = Tree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("ValueRow"));
            Row->AddChildToHorizontalBox(Text(Tree, TEXT("WarmthLabel"), TEXT("Warmth"), 19, Ivory))->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
            Row->AddChildToHorizontalBox(Text(Tree, TEXT("ValueText"), TEXT("68%"), 23, Ivory)); Add(Content, Row, 6);
            auto* Height = Tree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("SliderHeight")); Height->SetHeightOverride(32);
            auto* Overlay = Tree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("SliderLayers")); Height->AddChild(Overlay);
            auto* Fill = Tree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass(), TEXT("WarmthFill"));
            Fill->SetPercent(0.68f); Fill->SetFillColorAndOpacity(Amber); Fill->SetVisibility(ESlateVisibility::HitTestInvisible);
            auto PS = Fill->GetWidgetStyle(); PS.BackgroundImage = Rounded(FLinearColor(0.08f, 0.17f, 0.16f), 6); PS.FillImage = Rounded(FLinearColor::White, 6); Fill->SetWidgetStyle(PS);
            auto* FS = Overlay->AddChildToOverlay(Fill); FS->SetHorizontalAlignment(HAlign_Fill); FS->SetVerticalAlignment(VAlign_Center); FS->SetPadding(FMargin(10, 11));
            auto* Control = Tree->ConstructWidget<USlider>(USlider::StaticClass(), TEXT("WarmthSlider"));
            Control->IsFocusable = true; Control->RequiresControllerLock = false; Control->SetMinValue(0); Control->SetMaxValue(1); Control->SetStepSize(0.01f); Control->SetValue(0.68f);
            auto SS = Control->GetWidgetStyle(); SS.NormalBarImage.TintColor = FSlateColor(FLinearColor::Transparent); SS.HoveredBarImage = SS.NormalBarImage; SS.DisabledBarImage = SS.NormalBarImage;
            SS.NormalThumbImage = Rounded(Ivory, 12); SS.NormalThumbImage.ImageSize = FVector2D(24,24); SS.HoveredThumbImage = SS.NormalThumbImage; SS.DisabledThumbImage = SS.NormalThumbImage;
            Control->SetWidgetStyle(SS); auto* ControlSlot = Overlay->AddChildToOverlay(Control); ControlSlot->SetHorizontalAlignment(HAlign_Fill); ControlSlot->SetVerticalAlignment(VAlign_Fill); Add(Content, Height, 4);
            auto* Ends = Tree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("Endpoints"));
            Ends->AddChildToHorizontalBox(Text(Tree, TEXT("CoolLabel"), TEXT("Cool"), 13, Ivory))->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
            Ends->AddChildToHorizontalBox(Text(Tree, TEXT("WarmLabel"), TEXT("Warm"), 13, Ivory)); Add(Content, Ends, 8);
            auto* Preview = Tree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("LightPreview")); Preview->SetBrush(Rounded(FLinearColor::White, 8)); Preview->SetPadding(FMargin(8));
            Preview->AddChild(Text(Tree, TEXT("PreviewLabel"), TEXT("LIGHT SAMPLE"), 11, Ink));
            auto* PreviewSize = Tree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("PreviewSize"));
            PreviewSize->SetHeightOverride(72.f);
            PreviewSize->AddChild(Preview);
            Add(Content, PreviewSize, 8);
            auto* FooterSpace = Tree->ConstructWidget<USpacer>(USpacer::StaticClass(), TEXT("FooterSpace"));
            Add(Content, FooterSpace, 0, true);
        }
        Add(Content, Line(Tree, TEXT("FooterRule"), Slider ? FLinearColor(0.05f,0.20f,0.18f) : FLinearColor(0.60f,0.58f,0.53f)), 8);
        auto* Footer = Tree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("Footer"));
        auto* HintSlot = Footer->AddChildToHorizontalBox(Text(Tree, TEXT("FooterHint"), Slider ? TEXT("Live preview") : TEXT("You're good to go."), 12, Slider ? Ivory : Ink));
        HintSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill)); HintSlot->SetVerticalAlignment(VAlign_Center);
        auto* CloseBox = Tree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("CloseSize")); CloseBox->SetWidthOverride(112); CloseBox->SetHeightOverride(42);
        CloseBox->AddChild(Button(Tree, TEXT("CloseButton"), TEXT("Close"), Slider ? Amber : Cobalt, Slider ? Ink : FLinearColor::White));
        Footer->AddChildToHorizontalBox(CloseBox); Add(Content, Footer, 0);
    }
    // Keep compiler identity metadata in sync with the rebuilt, animation-free tree.
    TArray<UWidget*> Widgets;
    Tree->GetAllWidgets(Widgets);
    TSet<FName> CurrentNames;
    for (const UWidget* Widget : Widgets)
    {
        const FName Name = Widget->GetFName();
        CurrentNames.Add(Name);
        if (!BP->WidgetVariableNameToGuidMap.Contains(Name))
            BP->WidgetVariableNameToGuidMap.Add(Name, FGuid::NewGuid());
    }
    for (auto It = BP->WidgetVariableNameToGuidMap.CreateIterator(); It; ++It)
    {
        if (!CurrentNames.Contains(It.Key())) It.RemoveCurrent();
    }
    FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(BP);
    FKismetEditorUtilities::CompileBlueprint(BP);
    if (BP->Status == BS_Error) return false;
    if (auto* Defaults = Cast<UUserWidget>(BP->GeneratedClass->GetDefaultObject()))
        Defaults->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    auto* Package = BP->GetOutermost();
    Package->MarkPackageDirty();
    FSavePackageArgs Args;
    Args.TopLevelFlags = RF_Public | RF_Standalone;
    return UPackage::SavePackage(Package, BP,
        *FPackageName::LongPackageNameToFilename(Path, FPackageName::GetAssetPackageExtension()), Args);
}
}
bool UGenerateSolPanelAssets::GenerateWidgets()
{
    return SolPanelAssets::Generate(TEXT("WBP_Main_GPT61Sol"), UMainWidget_GPT61Sol::StaticClass(), 0)
        && SolPanelAssets::Generate(TEXT("WBP_Popup_GPT61Sol"), UPopupWidget_GPT61Sol::StaticClass(), 1)
        && SolPanelAssets::Generate(TEXT("WBP_SliderPopup_GPT61Sol"), USliderPopupWidget_GPT61Sol::StaticClass(), 2);
}

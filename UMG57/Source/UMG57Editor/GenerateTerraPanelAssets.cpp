// Model: GPT-6.1 Terra; reasoning: Medium
// Editor-only authoring; runtime widgets do not construct their trees.
#include "GenerateTerraPanelAssets.h"
#include "WidgetBlueprint.h"
#include "WidgetBlueprintFactory.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/BackgroundBlur.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "Components/ProgressBar.h"
#include "Brushes/SlateRoundedBoxBrush.h"
#include "Terra/MainWidget_GPT61Terra.h"
#include "Terra/PopupWidget_GPT61Terra.h"
#include "Terra/SliderPopupWidget_GPT61Terra.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"

namespace
{
const FLinearColor Ivory(0.92f, 0.86f, 0.76f, 0.94f), Ink(0.12f, 0.09f, 0.10f, 1), Brass(0.52f, 0.31f, 0.12f, 1);
const FLinearColor Cyan(0.01f, 0.82f, 0.88f, 1), White(0.92f, 0.96f, 1, 1);
UTextBlock* Text(UWidgetTree* Tree, const TCHAR* Name, const TCHAR* Content, int32 Size, FLinearColor Color)
{
    UTextBlock* W = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), FName(Name));
    W->SetText(FText::FromString(Content));
    W->SetColorAndOpacity(FSlateColor(Color));
    W->SetJustification(ETextJustify::Center);
    W->SetAutoWrapText(true);
    FSlateFontInfo Font = W->GetFont(); Font.Size = Size; W->SetFont(Font);
    return W;
}
void AddText(UVerticalBox* Box, UTextBlock* W, float Bottom)
{
    auto* Slot = Box->AddChildToVerticalBox(W); Slot->SetHorizontalAlignment(HAlign_Fill); Slot->SetPadding(FMargin(0, 0, 0, Bottom));
}
UButton* Button(UWidgetTree* Tree, const TCHAR* Name, const TCHAR* Label, FLinearColor Fill, FLinearColor LabelColor)
{
    UButton* B = Tree->ConstructWidget<UButton>(UButton::StaticClass(), FName(Name));
    FButtonStyle Style = B->GetStyle();
    Style.SetNormal(FSlateRoundedBoxBrush(Fill, 10.0f));
    Style.SetHovered(FSlateRoundedBoxBrush(Fill * 1.12f, 10.0f));
    Style.SetPressed(FSlateRoundedBoxBrush(Fill * 0.85f, 10.0f));
    B->SetStyle(Style);
    B->SetColorAndOpacity(FLinearColor::White);
    B->AddChild(Text(Tree, TEXT("ButtonLabel"), Label, 16, LabelColor));
    return B;
}
void MainButton(UWidgetTree* Tree, UCanvasPanel* Root, const TCHAR* Name, const TCHAR* Label, float Y)
{
    UButton* B = Tree->ConstructWidget<UButton>(UButton::StaticClass(), FName(Name));
    auto* Slot = Root->AddChildToCanvas(B); Slot->SetAnchors(FAnchors(5.0f / 6.0f, 0)); Slot->SetAlignment(FVector2D(0.5f, 0.5f)); Slot->SetPosition(FVector2D(0, Y)); Slot->SetSize(FVector2D(300, 52));
    const FName LabelName = MakeUniqueObjectName(Tree, UTextBlock::StaticClass(), TEXT("OpenLabel"));
    UTextBlock* LabelWidget = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), LabelName); LabelWidget->SetText(FText::FromString(Label)); LabelWidget->SetJustification(ETextJustify::Center); B->AddChild(LabelWidget);
}
enum class EPanel { Main, Note, Slider };
bool UpdateWidget(const TCHAR* Name, UClass* Parent, EPanel Type)
{
    const FString Path = FString(TEXT("/Game/UI/Terra/")) + Name;
    UWidgetBlueprint* BP = LoadObject<UWidgetBlueprint>(nullptr, *(Path + TEXT(".") + Name));
    if (!BP)
    {
        UPackage* Package = CreatePackage(*Path);
        UWidgetBlueprintFactory* Factory = NewObject<UWidgetBlueprintFactory>(); Factory->ParentClass = Parent;
        BP = Cast<UWidgetBlueprint>(Factory->FactoryCreateNew(UWidgetBlueprint::StaticClass(), Package, FName(Name), RF_Public | RF_Standalone, nullptr, GWarn));
        if (BP) FAssetRegistryModule::AssetCreated(BP);
    }
    if (!BP || !BP->WidgetTree || BP->ParentClass != Parent) return false;
    UWidgetTree* Tree = BP->WidgetTree;
    TArray<UWidget*> OldWidgets; Tree->GetAllWidgets(OldWidgets); Tree->RootWidget = nullptr;
    for (UWidget* Widget : OldWidgets) Widget->Rename(nullptr, GetTransientPackage(), REN_DontCreateRedirectors | REN_NonTransactional);
    UCanvasPanel* Root = Tree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("Root")); Root->SetVisibility(ESlateVisibility::SelfHitTestInvisible); Tree->RootWidget = Root;
    if (Type == EPanel::Main)
    {
        MainButton(Tree, Root, TEXT("OpenButton"), TEXT("Open Terra note"), 60);
        MainButton(Tree, Root, TEXT("SliderOpenButton"), TEXT("Open Terra rain"), 125);
    }
    else
    {
        const bool bSlider = Type == EPanel::Slider;
        UBackgroundBlur* Blur = Tree->ConstructWidget<UBackgroundBlur>(UBackgroundBlur::StaticClass(), TEXT("CardBlur")); Blur->SetBlurStrength(bSlider ? 8 : 3); Blur->SetCornerRadius(FVector4(20, 20, 20, 20));
        auto* Slot = Root->AddChildToCanvas(Blur); Slot->SetAnchors(FAnchors(2.0f / 3.0f, 0.30f, 1.0f, 0.94f)); Slot->SetOffsets(FMargin(16, 0, 16, 0));
        UBorder* Card = Tree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Card"));
        Card->SetBrush(FSlateRoundedBoxBrush(bSlider ? FLinearColor(0.018f, 0.045f, 0.08f, 0.94f) : Ivory, 20.0f, bSlider ? FLinearColor(0.2f, 0.46f, 0.56f, 1) : Brass, 1.5f)); Card->SetPadding(FMargin(22, 14)); Blur->SetContent(Card);
        UVerticalBox* Content = Tree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("Content")); Card->SetContent(Content);
        if (!bSlider)
        {
            AddText(Content, Text(Tree, TEXT("Eyebrow"), TEXT("F I E L D  N O T E  0 7"), 13, Brass), 7);
            AddText(Content, Text(Tree, TEXT("Moon"), TEXT("*       (  O  )       *"), 24, Brass), 4);
            UTextBlock* Title = Text(Tree, TEXT("Title"), TEXT("A moment to look up"), 25, Ink);
            FSlateFontInfo Font = Title->GetFont(); Font.Size = 25; Font.TypefaceFontName = TEXT("Italic"); Title->SetFont(Font); AddText(Content, Title, 8);
            AddText(Content, Text(Tree, TEXT("Ornament"), TEXT("----   *   ----"), 14, Brass), 8);
            AddText(Content, Text(Tree, TEXT("Message"), TEXT("The sky is clearest when you take your time. Pause here, then return when you are ready."), 15, Ink), 12);
            USizeBox* Spacer = Tree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("NoteSpacer"));
            Content->AddChildToVerticalBox(Spacer)->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
            UButton* Close = Button(Tree, TEXT("CloseButton"), TEXT("Close"), FLinearColor(0.045f,0.035f,0.10f,1), Ivory);
            Content->AddChildToVerticalBox(Close)->SetHorizontalAlignment(HAlign_Fill);
        }
        else
        {
            AddText(Content, Text(Tree, TEXT("Eyebrow"), TEXT("A M B I E N C E"), 13, Cyan), 8);
            AddText(Content, Text(Tree, TEXT("Title"), TEXT("Rain intensity"), 26, White), 8);
            AddText(Content, Text(Tree, TEXT("Message"), TEXT("Set the sound of rainfall to suit your space."), 15, White), 8);
            AddText(Content, Text(Tree, TEXT("ValueText"), TEXT("65%"), 28, Cyan), 8);
            UOverlay* Track = Tree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Track")); Content->AddChildToVerticalBox(Track)->SetPadding(FMargin(0, 0, 0, 4));
            UProgressBar* Fill = Tree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass(), TEXT("IntensityFill")); Fill->SetPercent(0.65f); Fill->SetFillColorAndOpacity(Cyan); Fill->SetVisibility(ESlateVisibility::HitTestInvisible); auto* FillSlot = Track->AddChildToOverlay(Fill); FillSlot->SetVerticalAlignment(VAlign_Center); FillSlot->SetHorizontalAlignment(HAlign_Fill);
            USlider* Slider = Tree->ConstructWidget<USlider>(USlider::StaticClass(), TEXT("IntensitySlider")); Slider->SetValue(0.65f); Slider->SetStepSize(0.05f); Slider->IsFocusable = true; Slider->RequiresControllerLock = false; Slider->SetSliderBarColor(FLinearColor(1,1,1,0)); Slider->SetSliderHandleColor(Cyan); auto* SliderSlot = Track->AddChildToOverlay(Slider); SliderSlot->SetHorizontalAlignment(HAlign_Fill);
            UHorizontalBox* Ends = Tree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("Endpoints")); Content->AddChildToVerticalBox(Ends)->SetPadding(FMargin(0, 0, 0, 7));
            auto* Quiet = Ends->AddChildToHorizontalBox(Text(Tree, TEXT("Quiet"), TEXT("Quiet"), 12, White)); Quiet->SetSize(FSlateChildSize(ESlateSizeRule::Fill)); Quiet->SetHorizontalAlignment(HAlign_Left);
            auto* Heavy = Ends->AddChildToHorizontalBox(Text(Tree, TEXT("Heavy"), TEXT("Heavy"), 12, White)); Heavy->SetHorizontalAlignment(HAlign_Right);
            UBorder* Preview = Tree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("RainPreview")); Preview->SetBrush(FSlateRoundedBoxBrush(FLinearColor::White, 5.0f)); Preview->SetBrushColor(FLinearColor(0.05f,0.75f,0.85f,0.5f)); Preview->SetPadding(FMargin(5)); Preview->SetContent(Text(Tree, TEXT("PreviewLabel"), TEXT("RAIN  /  /  /  /  /"), 13, White)); Content->AddChildToVerticalBox(Preview)->SetPadding(FMargin(0, 0, 0, 8));
            USizeBox* Spacer = Tree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("SliderSpacer"));
            Content->AddChildToVerticalBox(Spacer)->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
            UButton* Close = Button(Tree, TEXT("CloseButton"), TEXT("Close"), Cyan, FLinearColor(0.01f,0.04f,0.07f,1)); Content->AddChildToVerticalBox(Close)->SetHorizontalAlignment(HAlign_Fill);
        }
    }
    TArray<UWidget*> Widgets; Tree->GetAllWidgets(Widgets); TSet<FName> Names;
    for (const UWidget* Widget : Widgets) { Names.Add(Widget->GetFName()); if (!BP->WidgetVariableNameToGuidMap.Contains(Widget->GetFName())) BP->WidgetVariableNameToGuidMap.Add(Widget->GetFName(), FGuid::NewGuid()); }
    for (auto It = BP->WidgetVariableNameToGuidMap.CreateIterator(); It; ++It) if (!Names.Contains(It.Key())) It.RemoveCurrent();
    FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(BP); FKismetEditorUtilities::CompileBlueprint(BP); if (BP->Status == BS_Error) return false;
    BP->MarkPackageDirty(); const FString File = FPackageName::LongPackageNameToFilename(Path, FPackageName::GetAssetPackageExtension()); FSavePackageArgs Args; Args.TopLevelFlags = RF_Public | RF_Standalone;
    return UPackage::SavePackage(BP->GetOutermost(), BP, *File, Args);
}
}
bool UGenerateTerraPanelAssets::GenerateWidgets()
{
    return UpdateWidget(TEXT("WBP_Main_GPT61Terra"), UMainWidget_GPT61Terra::StaticClass(), EPanel::Main)
        && UpdateWidget(TEXT("WBP_Popup_GPT61Terra"), UPopupWidget_GPT61Terra::StaticClass(), EPanel::Note)
        && UpdateWidget(TEXT("WBP_SliderPopup_GPT61Terra"), USliderPopupWidget_GPT61Terra::StaticClass(), EPanel::Slider);
}

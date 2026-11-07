// Editor-only WBP authoring. Actual model: GPT-6 Astra; reasoning: Medium.
#include "GenerateAstraPanelAssets.h"
#include "WidgetBlueprint.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/BackgroundBlur.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "UObject/SavePackage.h"

namespace AstraPanel
{
FSlateBrush Rounded(FLinearColor Color, float Radius)
{
    FSlateBrush Brush;
    Brush.DrawAs = ESlateBrushDrawType::RoundedBox;
    Brush.TintColor = Color;
    Brush.OutlineSettings = FSlateBrushOutlineSettings(Radius);
    return Brush;
}

UButton* Button(UWidgetTree* Tree, const TCHAR* Name, const FString& Label)
{
    auto* Result = Tree->ConstructWidget<UButton>(UButton::StaticClass(), Name);
    FButtonStyle Style = Result->GetStyle();
    Style.SetNormal(Rounded(FLinearColor(.12f,.22f,.38f,.95f), 10));
    Style.SetHovered(Rounded(FLinearColor(.20f,.37f,.58f,1), 10));
    Style.SetPressed(Rounded(FLinearColor(.08f,.16f,.28f,1), 10));
    Result->SetStyle(Style);
    auto* Text = Tree->ConstructWidget<UTextBlock>();
    Text->SetText(FText::FromString(Label));
    auto Font = Text->GetFont(); Font.Size = 18; Text->SetFont(Font);
    Result->AddChild(Text);
    return Result;
}

bool Make(bool bPopup)
{
    const FString Path = bPopup ? TEXT("/Game/UI/Astra/WBP_Popup_GPT6Astra") : TEXT("/Game/UI/Astra/WBP_Main_GPT6Astra");
    auto* BP = LoadObject<UWidgetBlueprint>(nullptr, *Path);
    if (!BP || !BP->WidgetTree) return false;
    BP->WidgetTree->Rename(nullptr, GetTransientPackage(), REN_DontCreateRedirectors | REN_NonTransactional);
    BP->WidgetTree = NewObject<UWidgetTree>(BP, TEXT("WidgetTree"), RF_Transactional);
    auto* Tree = BP->WidgetTree.Get();
    auto* Root = Tree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("Root"));
    Root->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    Tree->RootWidget = Root;
    if (!bPopup)
    {
        auto* Open = Button(Tree, TEXT("OpenButton"), TEXT("Open GPT-6 Astra Popup"));
        auto* Slot = Root->AddChildToCanvas(Open);
        Slot->SetAnchors(FAnchors(1.f/6,0)); Slot->SetAlignment(FVector2D(.5,.5));
        Slot->SetPosition(FVector2D(0,80)); Slot->SetSize(FVector2D(300,60));
        auto* All = Button(Tree, TEXT("AllOpenButton"), TEXT("\uBAA8\uB4E0 \uD31D\uC5C5 \uC5F4\uAE30"));
        auto* AllSlot = Root->AddChildToCanvas(All);
        AllSlot->SetAnchors(FAnchors(.5,0)); AllSlot->SetAlignment(FVector2D(.5,.5));
        AllSlot->SetPosition(FVector2D(0,160)); AllSlot->SetSize(FVector2D(300,60));
    }
    else
    {
        auto* Blur = Tree->ConstructWidget<UBackgroundBlur>(UBackgroundBlur::StaticClass(), TEXT("PanelBlur"));
        Blur->SetBlurStrength(8); Blur->SetCornerRadius(FVector4(20,20,20,20));
        Blur->SetApplyAlphaToBlur(false);
        auto* Slot = Root->AddChildToCanvas(Blur);
        Slot->SetAnchors(FAnchors(0,.32f,1.f/3,.85f)); Slot->SetOffsets(FMargin(20,0,20,0));
        auto* Panel = Tree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("PanelSurface"));
        Panel->SetBrush(Rounded(FLinearColor(.025f,.045f,.075f,.62f),20));
        Panel->SetPadding(FMargin(24)); Blur->AddChild(Panel);
        auto* Column = Tree->ConstructWidget<UVerticalBox>(); Panel->AddChild(Column);
        auto* Title = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Title"));
        Title->SetText(FText::FromString(TEXT("GPT-6 Astra")));
        auto Font = Title->GetFont(); Font.Size=26; Title->SetFont(Font); Title->SetAutoWrapText(true);
        Column->AddChildToVerticalBox(Title)->SetPadding(FMargin(0,0,0,20));
        auto* Message = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Message"));
        Message->SetText(FText::FromString(TEXT("Astra popup\nOther panels and controls remain available.")));
        Message->SetAutoWrapText(true); Font.Size=18; Message->SetFont(Font);
        auto* BodySlot = Column->AddChildToVerticalBox(Message);
        BodySlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill)); BodySlot->SetPadding(FMargin(0,0,0,20));
        Column->AddChildToVerticalBox(Button(Tree,TEXT("CloseButton"),TEXT("Close")))->SetPadding(FMargin(0,8));
    }
    TArray<UWidget*> Widgets;
    Tree->GetAllWidgets(Widgets);
    TSet<FName> Names;
    for (auto* Widget : Widgets)
    {
        Names.Add(Widget->GetFName());
        if (!BP->WidgetVariableNameToGuidMap.Contains(Widget->GetFName()))
            BP->WidgetVariableNameToGuidMap.Add(Widget->GetFName(), FGuid::NewGuid());
    }
    for (auto It = BP->WidgetVariableNameToGuidMap.CreateIterator(); It; ++It)
        if (!Names.Contains(It.Key())) It.RemoveCurrent();
    FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(BP);
    FKismetEditorUtilities::CompileBlueprint(BP);
    if (BP->Status == BS_Error) return false;
    BP->MarkPackageDirty();
    FSavePackageArgs Args; Args.TopLevelFlags = RF_Public | RF_Standalone;
    const FString Filename = FPackageName::LongPackageNameToFilename(Path,FPackageName::GetAssetPackageExtension());
    return UPackage::SavePackage(BP->GetOutermost(),BP,*Filename,Args);
}
}

bool UGenerateAstraPanelAssets::GenerateWidgets() { return AstraPanel::Make(true) && AstraPanel::Make(false); }

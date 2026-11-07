// Model: GPT-6.1 Sol; reasoning: Medium
// Editor-only WBP asset authoring. Runtime widget classes never build trees.
#include "GenerateSolPanelAssets.h"
#include "WidgetBlueprint.h"
#include "WidgetBlueprintFactory.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/BackgroundBlur.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Sol/MainWidget_GPT61Sol.h"
#include "Sol/PopupWidget_GPT61Sol.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"
#include "Misc/PackageName.h"

namespace SolPanelAssets
{
UTextBlock* Text(UWidgetTree* Tree, const TCHAR* Name, const TCHAR* Value, int32 Size)
{
    auto* Widget = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), Name);
    Widget->SetText(FText::FromString(Value));
    Widget->SetAutoWrapText(true);
    auto Font = Widget->GetFont();
    Font.Size = Size;
    Widget->SetFont(Font);
    return Widget;
}
bool Generate(const TCHAR* Name, UClass* Parent, bool Popup)
{
    const FString Path = FString(TEXT("/Game/UI/Sol/")) + Name;
    auto* BP = LoadObject<UWidgetBlueprint>(nullptr, *Path);
    if (!BP)
    {
        auto* Package = CreatePackage(*Path);
        auto* Factory = NewObject<UWidgetBlueprintFactory>();
        Factory->ParentClass = Parent;
        BP = Cast<UWidgetBlueprint>(Factory->FactoryCreateNew(UWidgetBlueprint::StaticClass(),
            Package, FName(Name), RF_Public | RF_Standalone, nullptr, GWarn));
        if (!BP) return false;
        FAssetRegistryModule::AssetCreated(BP);
    }
    // Keep the existing parent and replace only its editor-authored widget tree.
    BP->Modify();
    BP->WidgetTree = NewObject<UWidgetTree>(BP, NAME_None, RF_Transactional);
    UWidgetTree* Tree = BP->WidgetTree.Get();
    auto* Root = Tree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("Root"));
    Root->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    Tree->RootWidget = Root;
    if (!Popup)
    {
        auto* Button = Tree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("OpenButton"));
        auto* Slot = Root->AddChildToCanvas(Button);
        Slot->SetAnchors(FAnchors(0.5f, 0.0f));
        Slot->SetAlignment(FVector2D(0.5f, 0.5f));
        Slot->SetPosition(FVector2D(0, 80));
        Slot->SetSize(FVector2D(300, 60));
        Button->AddChild(Text(Tree, TEXT("OpenLabel"), TEXT("GPT-6.1 Sol"), 20));
    }
    else
    {
        auto* Blur = Tree->ConstructWidget<UBackgroundBlur>(UBackgroundBlur::StaticClass(), TEXT("CardBlur"));
        Blur->SetBlurStrength(8.0f);
        Blur->SetCornerRadius(FVector4(20, 20, 20, 20));
        auto* Slot = Root->AddChildToCanvas(Blur);
        Slot->SetAnchors(FAnchors(1.0f / 3.0f, 0.32f, 2.0f / 3.0f, 0.85f));
        Slot->SetOffsets(FMargin(20, 0, 20, 0));
        auto* Card = Tree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Card"));
        FSlateBrush Brush;
        Brush.DrawAs = ESlateBrushDrawType::RoundedBox;
        Brush.TintColor = FSlateColor(FLinearColor(0.025f, 0.035f, 0.055f, 0.72f));
        Brush.OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
        Brush.OutlineSettings.CornerRadii = FVector4(20, 20, 20, 20);
        Card->SetBrush(Brush);
        Card->SetPadding(FMargin(24));
        Blur->AddChild(Card);
        auto* Content = Tree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("Content"));
        Card->AddChild(Content);
        Content->AddChildToVerticalBox(Text(Tree, TEXT("Title"), TEXT("GPT-6.1 Sol"), 24))->SetPadding(FMargin(0, 0, 0, 16));
        auto* Body = Content->AddChildToVerticalBox(Text(Tree, TEXT("Message"),
            TEXT("This is a non-modal popup. You can interact with the other panels while it is open."), 18));
        Body->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
        Body->SetPadding(FMargin(0, 0, 0, 20));
        auto* Close = Tree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("CloseButton"));
        Close->AddChild(Text(Tree, TEXT("CloseLabel"), TEXT("Close"), 20));
        Content->AddChildToVerticalBox(Close)->SetPadding(FMargin(0, 8, 0, 0));
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
    return SolPanelAssets::Generate(TEXT("WBP_Main_GPT61Sol"), UMainWidget_GPT61Sol::StaticClass(), false)
        && SolPanelAssets::Generate(TEXT("WBP_Popup_GPT61Sol"), UPopupWidget_GPT61Sol::StaticClass(), true);
}

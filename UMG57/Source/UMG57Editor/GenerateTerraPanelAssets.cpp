// Model: GPT-6.1 Terra; reasoning: Medium
// Editor-only authoring. Runtime widget classes never construct a tree.
#include "GenerateTerraPanelAssets.h"
#include "WidgetBlueprint.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/BackgroundBlur.h"
#include "Components/Border.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Brushes/SlateRoundedBoxBrush.h"
#include "Terra/MainWidget_GPT61Terra.h"
#include "Terra/PopupWidget_GPT61Terra.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "UObject/SavePackage.h"

namespace
{
bool UpdateWidget(const TCHAR* Name, UClass* Parent, bool bPopup)
{
    const FString Path = FString(TEXT("/Game/UI/Terra/")) + Name;
    UWidgetBlueprint* BP = LoadObject<UWidgetBlueprint>(nullptr, *(Path + TEXT(".") + Name));
    if (!BP || !BP->WidgetTree || BP->ParentClass != Parent) return false;
    UWidgetTree* Tree = BP->WidgetTree;
    TArray<UWidget*> OldWidgets;
    Tree->GetAllWidgets(OldWidgets);
    Tree->RootWidget = nullptr;
    for (UWidget* Widget : OldWidgets)
    {
        Widget->Rename(nullptr, GetTransientPackage(), REN_DontCreateRedirectors | REN_NonTransactional);
    }
    UCanvasPanel* Root = Tree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("Root"));
    Root->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    Tree->RootWidget = Root;
    if (!bPopup)
    {
        UButton* Button = Tree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("OpenButton"));
        UCanvasPanelSlot* Slot = Root->AddChildToCanvas(Button);
        Slot->SetAnchors(FAnchors(5.0f / 6.0f, 0.0f));
        Slot->SetAlignment(FVector2D(0.5f, 0.5f));
        Slot->SetPosition(FVector2D(0, 80));
        Slot->SetSize(FVector2D(300, 60));
        UTextBlock* Label = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ButtonLabel"));
        Label->SetText(FText::FromString(TEXT("Open GPT-6.1 Terra")));
        Button->AddChild(Label);
    }
    else
    {
        UBackgroundBlur* Blur = Tree->ConstructWidget<UBackgroundBlur>(UBackgroundBlur::StaticClass(), TEXT("CardBlur"));
        Blur->SetBlurStrength(8.0f);
        Blur->SetCornerRadius(FVector4(20, 20, 20, 20));
        UCanvasPanelSlot* Slot = Root->AddChildToCanvas(Blur);
        Slot->SetAnchors(FAnchors(2.0f / 3.0f, 0.32f, 1.0f, 0.85f));
        Slot->SetOffsets(FMargin(20, 0, 20, 0));
        UBorder* Card = Tree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("Card"));
        Card->SetBrush(FSlateRoundedBoxBrush(FLinearColor(0.025f, 0.035f, 0.055f, 0.78f), 20.0f));
        Card->SetPadding(FMargin(24));
        Blur->SetContent(Card);
        UVerticalBox* Content = Tree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("Content"));
        Card->SetContent(Content);
        UTextBlock* Title = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Title"));
        Title->SetText(FText::FromString(TEXT("GPT-6.1 Terra")));
        FSlateFontInfo TitleFont = Title->GetFont(); TitleFont.Size = 24; Title->SetFont(TitleFont);
        Content->AddChildToVerticalBox(Title)->SetPadding(FMargin(0, 0, 0, 16));
        UTextBlock* Message = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("Message"));
        Message->SetText(FText::FromString(TEXT("This is the GPT-6.1 Terra popup.")));
        Message->SetAutoWrapText(true);
        Content->AddChildToVerticalBox(Message)->SetPadding(FMargin(0, 0, 0, 20));
        UButton* Close = Tree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("CloseButton"));
        Content->AddChildToVerticalBox(Close)->SetHorizontalAlignment(HAlign_Left);
        UTextBlock* Label = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("ButtonLabel"));
        Label->SetText(FText::FromString(TEXT("Close")));
        Close->AddChild(Label);
    }
    TArray<UWidget*> NewWidgets;
    Tree->GetAllWidgets(NewWidgets);
    TSet<FName> NewNames;
    for (const UWidget* Widget : NewWidgets)
    {
        NewNames.Add(Widget->GetFName());
        if (!BP->WidgetVariableNameToGuidMap.Contains(Widget->GetFName()))
            BP->WidgetVariableNameToGuidMap.Add(Widget->GetFName(), FGuid::NewGuid());
    }
    for (auto It = BP->WidgetVariableNameToGuidMap.CreateIterator(); It; ++It)
        if (!NewNames.Contains(It.Key())) It.RemoveCurrent();
    FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(BP);
    FKismetEditorUtilities::CompileBlueprint(BP);
    if (BP->Status == BS_Error) return false;
    BP->MarkPackageDirty();
    const FString File = FPackageName::LongPackageNameToFilename(Path, FPackageName::GetAssetPackageExtension());
    FSavePackageArgs Args; Args.TopLevelFlags = RF_Public | RF_Standalone;
    return UPackage::SavePackage(BP->GetOutermost(), BP, *File, Args);
}
}
bool UGenerateTerraPanelAssets::GenerateWidgets()
{
    return UpdateWidget(TEXT("WBP_Main_GPT61Terra"), UMainWidget_GPT61Terra::StaticClass(), false)
        && UpdateWidget(TEXT("WBP_Popup_GPT61Terra"), UPopupWidget_GPT61Terra::StaticClass(), true);
}

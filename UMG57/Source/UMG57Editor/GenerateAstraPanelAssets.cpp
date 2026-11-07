// Editor-only WBP authoring. Actual model: GPT-6 Astra; reasoning effort: Medium.
#include "GenerateAstraPanelAssets.h"
#include "WidgetBlueprint.h"
#include "WidgetBlueprintFactory.h"
#include "Blueprint/WidgetTree.h"
#include "Astra/MainWidget_GPT6Astra.h"
#include "Astra/PopupWidget_GPT6Astra.h"
#include "Astra/VolumePopupWidget_GPT6Astra.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/BackgroundBlur.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/SizeBox.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "Components/Slider.h"
#include "Components/ProgressBar.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Misc/PackageName.h"
#include "UObject/SavePackage.h"

namespace AstraPanel
{
const FLinearColor Navy(.022f,.045f,.09f,1), Ink(.035f,.04f,.05f,1), Paper(.93f,.86f,.73f,.94f), Copper(.48f,.24f,.12f,1), Mint(.48f,.87f,.55f,1), Cream(.94f,.92f,.83f,1);
FSlateBrush Rounded(FLinearColor Color, float Radius)
{
    FSlateBrush Brush; Brush.DrawAs = ESlateBrushDrawType::RoundedBox;
    Brush.TintColor = Color; Brush.OutlineSettings = FSlateBrushOutlineSettings(Radius); return Brush;
}
UTextBlock* Text(UWidgetTree* T, FName Name, const TCHAR* Value, int32 Size, FLinearColor Color, FName Face=TEXT("Regular"))
{
    auto* W = T->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(),Name);
    W->SetText(FText::FromString(Value)); W->SetColorAndOpacity(Color); W->SetAutoWrapText(true);
    auto F=W->GetFont(); F.Size=Size; F.TypefaceFontName=Face; W->SetFont(F); return W;
}
UButton* Button(UWidgetTree* T, const TCHAR* Name, const TCHAR* Label, FLinearColor Bg=Navy, FLinearColor Fg=Cream)
{
    auto* B=T->ConstructWidget<UButton>(UButton::StaticClass(),Name);
    FButtonStyle S=B->GetStyle(); S.SetNormal(Rounded(Bg,10));
    auto Hover=Bg*1.3f; Hover.A=1; S.SetHovered(Rounded(Hover,10)); S.SetPressed(Rounded(Bg*.8f,10));
    S.SetNormalPadding(FMargin(16,10)); S.SetPressedPadding(FMargin(16,11,16,9)); B->SetStyle(S);
    B->AddChild(Text(T,NAME_None,Label,18,Fg,TEXT("Bold"))); return B;
}
UBorder* Surface(UWidgetTree* T, const TCHAR* Name, FLinearColor Color, float Radius, FMargin Padding)
{
    auto* B=T->ConstructWidget<UBorder>(UBorder::StaticClass(),Name); B->SetBrush(Rounded(Color,Radius)); B->SetPadding(Padding); return B;
}
void Row(UVerticalBox* V,UWidget* W,float Bottom=12)
{
    V->AddChildToVerticalBox(W)->SetPadding(FMargin(0,0,0,Bottom));
}
void Fill(UVerticalBox* V,UWidget* W)
{
    auto* S=V->AddChildToVerticalBox(W); S->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
}
bool Make(int32 Kind)
{
    const TCHAR* Name=Kind==0?TEXT("WBP_Main_GPT6Astra"):Kind==1?TEXT("WBP_Popup_GPT6Astra"):TEXT("WBP_VolumePopup_GPT6Astra");
    const FString Path=FString(TEXT("/Game/UI/Astra/"))+Name;
    UClass* Parent=Kind==0?UMainWidget_GPT6Astra::StaticClass():Kind==1?UPopupWidget_GPT6Astra::StaticClass():UVolumePopupWidget_GPT6Astra::StaticClass();
    auto* BP=LoadObject<UWidgetBlueprint>(nullptr,*Path);
    if (!BP)
    {
        auto* Factory=NewObject<UWidgetBlueprintFactory>(); Factory->ParentClass=Parent;
        BP=Cast<UWidgetBlueprint>(Factory->FactoryCreateNew(UWidgetBlueprint::StaticClass(),CreatePackage(*Path),FName(Name),RF_Public|RF_Standalone,nullptr,GWarn));
        if (!BP) return false; FAssetRegistryModule::AssetCreated(BP);
    }
    if (BP->ParentClass!=Parent) return false;
    if (BP->WidgetTree) BP->WidgetTree->Rename(nullptr,GetTransientPackage(),REN_DontCreateRedirectors|REN_NonTransactional);
    BP->WidgetTree=NewObject<UWidgetTree>(BP,TEXT("WidgetTree"),RF_Transactional);
    auto* T=BP->WidgetTree.Get(); auto* Root=T->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(),TEXT("Root"));
    Root->SetVisibility(ESlateVisibility::SelfHitTestInvisible); T->RootWidget=Root;
    if (Kind==0)
    {
        auto Add=[&](const TCHAR* N,const TCHAR* L,float X,float Y,float Width)
        {
            auto* S=Root->AddChildToCanvas(Button(T,N,L)); S->SetAnchors(FAnchors(X,0));
            S->SetAlignment(FVector2D(.5,.5)); S->SetPosition(FVector2D(0,Y)); S->SetSize(FVector2D(Width,52));
        };
        Add(TEXT("OpenButton"),TEXT("Astra / Observation"),1.f/6,60,300);
        Add(TEXT("SliderOpenButton"),TEXT("Astra / Volume"),1.f/6,125,300);
        Add(TEXT("AllOpenButton"),TEXT("\uBAA8\uB4E0 \uD31D\uC5C5 \uC5F4\uAE30"),.39f,210,300);
        Add(TEXT("AllSlidersButton"),TEXT("\uBAA8\uB4E0 \uC2AC\uB77C\uC774\uB354 \uC5F4\uAE30"),.61f,210,300);
    }
    else
    {
        const bool Slider=Kind==2;
        auto* Blur=T->ConstructWidget<UBackgroundBlur>(UBackgroundBlur::StaticClass(),TEXT("PanelBlur"));
        Blur->SetBlurStrength(8); Blur->SetCornerRadius(FVector4(20,20,20,20)); Blur->SetApplyAlphaToBlur(false);
        auto* CS=Root->AddChildToCanvas(Blur); CS->SetAnchors(FAnchors(0,.30f,1.f/3,.94f)); CS->SetOffsets(FMargin(16,0,16,0));
        auto* Panel=Surface(T,TEXT("PanelSurface"),Slider?FLinearColor(.035f,.045f,.03f,.88f):Paper,20,FMargin(24));
        Blur->AddChild(Panel); auto* V=T->ConstructWidget<UVerticalBox>(); Panel->AddChild(V);
        auto* Head=T->ConstructWidget<UHorizontalBox>();
        auto* Eyebrow=Text(T,TEXT("Eyebrow"),Slider?TEXT("SOUND SETTINGS"):TEXT("OBSERVATION LOG"),14,Slider?Mint:Copper,TEXT("Bold"));
        auto* HS=Head->AddChildToHorizontalBox(Eyebrow); HS->SetSize(FSlateChildSize(ESlateSizeRule::Fill)); HS->SetVerticalAlignment(VAlign_Center);
        Head->AddChildToHorizontalBox(Button(T,TEXT("DismissButton"),TEXT("x"),Slider?FLinearColor(.09f,.10f,.08f,1):Navy)); Row(V,Head,16);
        if (!Slider)
        {
            auto* Orbit=Surface(T,TEXT("OrbitBanner"),Navy,12,FMargin(14,12));
            auto* Art=Text(T,TEXT("OrbitMark"),TEXT("- - - -   O   - - - -"),30,FLinearColor(.72f,.47f,.25f,1),TEXT("Light")); Art->SetJustification(ETextJustify::Center); Orbit->AddChild(Art); Row(V,Orbit,16);
        }
        Row(V,Text(T,TEXT("Title"),Slider?TEXT("Find your quiet."):TEXT("A new\nperspective."),34,Slider?Cream:Ink,Slider?TEXT("Bold"):TEXT("Light")),14);
        Row(V,Text(T,TEXT("Message"),Slider?TEXT("Set the background sound to a level that helps you focus."):TEXT("Your observation has been saved. Return whenever you are ready to explore the details."),17,Slider?Cream:Ink),20);
        if (Slider)
        {
            auto* Inset=Surface(T,TEXT("ControlSurface"),FLinearColor(.085f,.10f,.07f,.8f),12,FMargin(16));
            auto* C=T->ConstructWidget<UVerticalBox>(); Inset->AddChild(C);
            auto* LabelRow=T->ConstructWidget<UHorizontalBox>();
            LabelRow->AddChildToHorizontalBox(Text(T,NAME_None,TEXT("Ambient volume"),18,Cream,TEXT("Bold")))->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
            LabelRow->AddChildToHorizontalBox(Text(T,TEXT("ValueText"),TEXT("65%"),28,Cream,TEXT("Bold"))); Row(C,LabelRow,12);
            auto* Track=T->ConstructWidget<UOverlay>();
            auto* Meter=T->ConstructWidget<UProgressBar>(UProgressBar::StaticClass(),TEXT("LevelPreview"));
            FProgressBarStyle PS; PS.SetBackgroundImage(Rounded(FLinearColor(.22f,.23f,.20f,1),4)); PS.SetFillImage(Rounded(FLinearColor::White,4)); PS.BackgroundImage.ImageSize=FVector2D(32,8); PS.FillImage.ImageSize=FVector2D(32,8); Meter->SetWidgetStyle(PS); Meter->SetFillColorAndOpacity(Mint); Meter->SetPercent(.65f);
            auto* MS=Track->AddChildToOverlay(Meter); MS->SetHorizontalAlignment(HAlign_Fill); MS->SetVerticalAlignment(VAlign_Center); MS->SetPadding(FMargin(10,0));
            auto* S=T->ConstructWidget<USlider>(USlider::StaticClass(),TEXT("VolumeSlider"));
            FSliderStyle Style; auto Empty=Rounded(FLinearColor::Transparent,0); auto Thumb=Rounded(FLinearColor::White,12); Thumb.ImageSize=FVector2D(24,24);
            Style.SetNormalBarImage(Empty); Style.SetHoveredBarImage(Empty); Style.SetDisabledBarImage(Empty); Style.SetNormalThumbImage(Thumb); Style.SetHoveredThumbImage(Thumb); Style.SetDisabledThumbImage(Thumb); Style.SetBarThickness(8);
            S->SetWidgetStyle(Style); S->SetSliderHandleColor(Mint); S->SetValue(.65f); S->SetStepSize(.01f); S->RequiresControllerLock=false;
            auto* SS=Track->AddChildToOverlay(S); SS->SetHorizontalAlignment(HAlign_Fill); SS->SetVerticalAlignment(VAlign_Center); Row(C,Track,8);
            auto* Ends=T->ConstructWidget<UHorizontalBox>(); Ends->AddChildToHorizontalBox(Text(T,NAME_None,TEXT("Silent"),14,Cream))->SetSize(FSlateChildSize(ESlateSizeRule::Fill)); Ends->AddChildToHorizontalBox(Text(T,NAME_None,TEXT("Full"),14,Cream)); Row(C,Ends,0); Row(V,Inset,18);
            Fill(V,Text(T,TEXT("PreviewNote"),TEXT("Visual level preview\nSaved for this session."),14,Cream));
        }
        else
        {
            auto* Meta=Surface(T,TEXT("MetadataSurface"),FLinearColor(.72f,.61f,.44f,.18f),8,FMargin(12));
            auto* M=Text(T,TEXT("Metadata"),TEXT("LOCATION  /  Northern sky\nSTATUS       /  Archived"),15,Ink); Meta->AddChild(M); Row(V,Meta,14);
            Fill(V,Text(T,TEXT("JournalNote"),TEXT("Saved to your journal"),15,Copper,TEXT("Italic")));
        }
        auto* Foot=T->ConstructWidget<UHorizontalBox>();
        Foot->AddChildToHorizontalBox(Button(T,TEXT("CloseButton"),TEXT("Close"),Slider?FLinearColor(.10f,.12f,.09f,1):Navy))->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
        if (Slider) { auto* DS=Foot->AddChildToHorizontalBox(Button(T,TEXT("DoneButton"),TEXT("Done"),Mint,Ink)); DS->SetSize(FSlateChildSize(ESlateSizeRule::Fill)); DS->SetPadding(FMargin(12,0,0,0)); }
        Row(V,Foot,0);
    }
    TArray<UWidget*> Widgets; T->GetAllWidgets(Widgets); TSet<FName> Names;
    for (auto* W:Widgets) { Names.Add(W->GetFName()); if (!BP->WidgetVariableNameToGuidMap.Contains(W->GetFName())) BP->WidgetVariableNameToGuidMap.Add(W->GetFName(),FGuid::NewGuid()); }
    for (auto It=BP->WidgetVariableNameToGuidMap.CreateIterator();It;++It) if (!Names.Contains(It.Key())) It.RemoveCurrent();
    FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(BP); FKismetEditorUtilities::CompileBlueprint(BP);
    if (BP->Status==BS_Error) return false;
    BP->MarkPackageDirty(); FSavePackageArgs Args; Args.TopLevelFlags=RF_Public|RF_Standalone;
    return UPackage::SavePackage(BP->GetOutermost(),BP,*FPackageName::LongPackageNameToFilename(Path,FPackageName::GetAssetPackageExtension()),Args);
}
}
bool UGenerateAstraPanelAssets::GenerateWidgets() { return AstraPanel::Make(1) && AstraPanel::Make(2) && AstraPanel::Make(0); }

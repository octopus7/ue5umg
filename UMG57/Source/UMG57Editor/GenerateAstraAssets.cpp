// Editor-only asset authoring; the widget classes never construct the tree.
#include "GenerateAstraAssets.h"
#include "WidgetBlueprint.h"
#include "WidgetBlueprintFactory.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Astra/MainWidget_GPT6Astra.h"
#include "Astra/PopupWidget_GPT6Astra.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"
#include "UObject/UnrealType.h"
static bool MakeWidget(const TCHAR* Name,UClass* Parent,bool Popup){
 FString Path=FString(TEXT("/Game/UI/Astra/"))+Name; if (LoadObject<UWidgetBlueprint>(nullptr, *Path)) return true; UPackage* P=CreatePackage(*Path); auto* F=NewObject<UWidgetBlueprintFactory>(); F->ParentClass=Parent;
 auto* BP=Cast<UWidgetBlueprint>(F->FactoryCreateNew(UWidgetBlueprint::StaticClass(),P,FName(Name),RF_Public|RF_Standalone,nullptr,GWarn)); if(!BP||!BP->WidgetTree)return false;
 auto* Root=BP->WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(),TEXT("Root")); BP->WidgetTree->RootWidget=Root;
 if(Popup){ auto* Shade=BP->WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(),TEXT("Backdrop")); Shade->SetBrushColor(FLinearColor(0,0,0,0.75)); auto* S=Root->AddChildToCanvas(Shade); S->SetAnchors(FAnchors(0,0,1,1)); S->SetOffsets(FMargin(0)); auto* T=BP->WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(),TEXT("Message")); T->SetText(FText::FromString(TEXT("This is a modal popup."))); auto* M=Root->AddChildToCanvas(T); M->SetAnchors(FAnchors(0.5,0.5)); M->SetAlignment(FVector2D(0.5,0.5)); M->SetPosition(FVector2D(0,-50)); M->SetAutoSize(true); }
 auto* B=BP->WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(),Popup?TEXT("CloseButton"):TEXT("OpenButton")); auto* S=Root->AddChildToCanvas(B); S->SetAnchors(FAnchors(0.5,0.5)); S->SetAlignment(FVector2D(0.5,0.5)); S->SetSize(FVector2D(220,60)); auto* L=BP->WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(),TEXT("ButtonLabel")); L->SetText(FText::FromString(Popup?TEXT("Close"):TEXT("Open Popup"))); B->AddChild(L);
 for (EUINavigation Direction : {EUINavigation::Up, EUINavigation::Down, EUINavigation::Left, EUINavigation::Right, EUINavigation::Next, EUINavigation::Previous}) B->SetNavigationRuleExplicit(Direction, B);
 FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(BP); FKismetEditorUtilities::CompileBlueprint(BP); if (!Popup) { auto* Prop=FindFProperty<FClassProperty>(BP->GeneratedClass,TEXT("PopupClass")); auto* PopupType=LoadClass<UPopupWidget_GPT6Astra>(nullptr,TEXT("/Game/UI/Astra/WBP_Popup_GPT6Astra.WBP_Popup_GPT6Astra_C")); if (!Prop || !PopupType) return false; Prop->SetObjectPropertyValue_InContainer(BP->GeneratedClass->GetDefaultObject(),PopupType); }
 FAssetRegistryModule::AssetCreated(BP); P->MarkPackageDirty(); FString File=FPackageName::LongPackageNameToFilename(Path,FPackageName::GetAssetPackageExtension()); FSavePackageArgs A; A.TopLevelFlags=RF_Public|RF_Standalone; return UPackage::SavePackage(P,BP,*File,A);
}
bool UGenerateAstraAssets::GenerateWidgets(){return MakeWidget(TEXT("WBP_Popup_GPT6Astra"),UPopupWidget_GPT6Astra::StaticClass(),true)&&MakeWidget(TEXT("WBP_Main_GPT6Astra"),UMainWidget_GPT6Astra::StaticClass(),false);}

bool UGenerateAstraAssets::ConfigureModelButtons()
{
    const TCHAR* Folders[] = {TEXT("Astra"), TEXT("Sol"), TEXT("Terra")};
    const TCHAR* Suffixes[] = {TEXT("GPT6Astra"), TEXT("GPT61Sol"), TEXT("GPT61Terra")};
    const TCHAR* Models[] = {TEXT("GPT-6 Astra"), TEXT("GPT-6.1 Sol"), TEXT("GPT-6.1 Terra")};
    for (int32 Index = 0; Index < 3; ++Index)
    {
        for (bool bPopup : {false, true})
        {
            const FString Path = FString::Printf(TEXT("/Game/UI/%s/WBP_%s_%s"),
                Folders[Index], bPopup ? TEXT("Popup") : TEXT("Main"), Suffixes[Index]);
            auto* BP = LoadObject<UWidgetBlueprint>(nullptr, *Path);
            if (!BP || !BP->WidgetTree) return false;
            if (bPopup)
            {
                auto* Message = Cast<UTextBlock>(BP->WidgetTree->FindWidget(TEXT("Message")));
                if (!Message) return false;
                Message->SetText(FText::FromString(FString::Printf(TEXT("%s modal popup"), Models[Index])));
            }
            else
            {
                auto* Label = Cast<UTextBlock>(BP->WidgetTree->FindWidget(TEXT("ButtonLabel")));
                auto* Button = Cast<UButton>(BP->WidgetTree->FindWidget(TEXT("OpenButton")));
                auto* Slot = Button ? Cast<UCanvasPanelSlot>(Button->Slot) : nullptr;
                if (!Label || !Slot) return false;
                Label->SetText(FText::FromString(FString::Printf(TEXT("Open %s Popup"), Models[Index])));
                Slot->SetPosition(FVector2D(0, (Index - 1) * 90));
                Slot->SetSize(FVector2D(380, 64));
                // Main screens share the viewport; navigation may reach the other model buttons.
                for (EUINavigation Direction : {EUINavigation::Up, EUINavigation::Down,
                    EUINavigation::Left, EUINavigation::Right, EUINavigation::Next, EUINavigation::Previous})
                    Button->SetNavigationRuleBase(Direction, EUINavigationRule::Escape);
            }
            FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(BP);
            FKismetEditorUtilities::CompileBlueprint(BP);
            if (BP->Status == BS_Error) return false;
            BP->MarkPackageDirty();
            FSavePackageArgs Args;
            Args.TopLevelFlags = RF_Public | RF_Standalone;
            const FString Filename = FPackageName::LongPackageNameToFilename(Path, FPackageName::GetAssetPackageExtension());
            if (!UPackage::SavePackage(BP->GetOutermost(), BP, *Filename, Args)) return false;
        }
    }
    return true;
}


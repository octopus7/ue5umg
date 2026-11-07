// Editor-only asset authoring; the widget classes never construct the tree.
#include "GenerateTestAssets.h"
#include "WidgetBlueprint.h"
#include "WidgetBlueprintFactory.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Terra/MainWidget_GPT61Terra.h"
#include "Terra/PopupWidget_GPT61Terra.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"
static bool MakeWidget(const TCHAR* Name,UClass* Parent,bool Popup){
 FString Path=FString(TEXT("/Game/UI/Terra/"))+Name; UPackage* P=CreatePackage(*Path); auto* F=NewObject<UWidgetBlueprintFactory>(); F->ParentClass=Parent;
 auto* BP=Cast<UWidgetBlueprint>(F->FactoryCreateNew(UWidgetBlueprint::StaticClass(),P,FName(Name),RF_Public|RF_Standalone,nullptr,GWarn)); if(!BP||!BP->WidgetTree)return false;
 auto* Root=BP->WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(),TEXT("Root")); BP->WidgetTree->RootWidget=Root;
 if(Popup){ auto* Shade=BP->WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(),TEXT("Backdrop")); Shade->SetBrushColor(FLinearColor(0,0,0,0.75)); auto* S=Root->AddChildToCanvas(Shade); S->SetAnchors(FAnchors(0,0,1,1)); S->SetOffsets(FMargin(0)); auto* T=BP->WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(),TEXT("Message")); T->SetText(FText::FromString(TEXT("This is a modal popup."))); auto* M=Root->AddChildToCanvas(T); M->SetAnchors(FAnchors(0.5,0.5)); M->SetAlignment(FVector2D(0.5,0.5)); M->SetPosition(FVector2D(0,-50)); M->SetAutoSize(true); }
 auto* B=BP->WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(),Popup?TEXT("CloseButton"):TEXT("OpenButton")); auto* S=Root->AddChildToCanvas(B); S->SetAnchors(FAnchors(0.5,0.5)); S->SetAlignment(FVector2D(0.5,0.5)); S->SetSize(FVector2D(220,60)); auto* L=BP->WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(),TEXT("ButtonLabel")); L->SetText(FText::FromString(Popup?TEXT("Close"):TEXT("Open Popup"))); B->AddChild(L);
 FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(BP); FKismetEditorUtilities::CompileBlueprint(BP); FAssetRegistryModule::AssetCreated(BP); P->MarkPackageDirty(); FString File=FPackageName::LongPackageNameToFilename(Path,FPackageName::GetAssetPackageExtension()); FSavePackageArgs A; A.TopLevelFlags=RF_Public|RF_Standalone; return UPackage::SavePackage(P,BP,*File,A);
}
bool UGenerateTestAssets::GenerateWidgets(){return MakeWidget(TEXT("WBP_Main_GPT61Terra"),UMainWidget_GPT61Terra::StaticClass(),false)&&MakeWidget(TEXT("WBP_Popup_GPT61Terra"),UPopupWidget_GPT61Terra::StaticClass(),true);}

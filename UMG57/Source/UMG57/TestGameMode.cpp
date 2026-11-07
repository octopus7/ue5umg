#include "TestGameMode.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
void ATestGameMode::BeginPlay(){ Super::BeginPlay(); auto* P=GetWorld()->GetFirstPlayerController(); auto C=LoadClass<UUserWidget>(nullptr,TEXT("/Game/UI/Terra/WBP_Main_GPT61Terra.WBP_Main_GPT61Terra_C")); if(P&&C){ auto* W=CreateWidget<UUserWidget>(P,C); W->AddToViewport(); P->bShowMouseCursor=true; FInputModeGameAndUI M; P->SetInputMode(M); } }
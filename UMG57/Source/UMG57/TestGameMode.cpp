#include "TestGameMode.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Astra/MainWidget_GPT6Astra.h"
void ATestGameMode::BeginPlay()
{
    Super::BeginPlay();
    auto* Player = GetWorld()->GetFirstPlayerController();
    if (!Player) return;
    Player->bShowMouseCursor = true;
    UMainWidget_GPT6Astra* AstraMain = nullptr;
    const TCHAR* MainPaths[] = {
        TEXT("/Game/UI/Astra/WBP_Main_GPT6Astra.WBP_Main_GPT6Astra_C"),
        TEXT("/Game/UI/Sol/WBP_Main_GPT61Sol.WBP_Main_GPT61Sol_C"),
        TEXT("/Game/UI/Terra/WBP_Main_GPT61Terra.WBP_Main_GPT61Terra_C")
    };
    // Each saved WBP owns its button layout and opens its original model's popup.
    for (const TCHAR* Path : MainPaths)
    {
        auto WidgetClass = LoadClass<UUserWidget>(nullptr, Path);
        if (!WidgetClass) continue;
        if (auto* Widget = CreateWidget<UUserWidget>(Player, WidgetClass))
        {
            Widget->AddToViewport();
            if (auto* Astra = Cast<UMainWidget_GPT6Astra>(Widget)) AstraMain = Astra;
        }
    }
    if (AstraMain) AstraMain->FocusOpener();
}

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestGameMode.generated.h"
UCLASS() class UMG57_API ATestGameMode:public AGameModeBase { GENERATED_BODY() public: virtual void BeginPlay() override; };
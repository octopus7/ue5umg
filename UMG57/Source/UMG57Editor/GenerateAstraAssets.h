#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GenerateAstraAssets.generated.h"
UCLASS() class UGenerateAstraAssets:public UBlueprintFunctionLibrary { GENERATED_BODY() public: UFUNCTION(BlueprintCallable,Category="Setup") static bool GenerateWidgets(); UFUNCTION(BlueprintCallable,Category="Setup") static bool ConfigureModelButtons(); };

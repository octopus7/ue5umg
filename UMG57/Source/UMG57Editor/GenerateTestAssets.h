#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GenerateTestAssets.generated.h"
UCLASS() class UGenerateTestAssets:public UBlueprintFunctionLibrary { GENERATED_BODY() public: UFUNCTION(BlueprintCallable,Category="Setup") static bool GenerateWidgets(); };
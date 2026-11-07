#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GenerateSolAssets.generated.h"
UCLASS() class UGenerateSolAssets:public UBlueprintFunctionLibrary { GENERATED_BODY() public: UFUNCTION(BlueprintCallable,Category="Setup") static bool GenerateWidgets(); };
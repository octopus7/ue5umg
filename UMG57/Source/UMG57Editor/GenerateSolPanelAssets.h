// Model: GPT-6.1 Sol; reasoning: Medium
#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GenerateSolPanelAssets.generated.h"
UCLASS()
class UGenerateSolPanelAssets : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="Setup") static bool GenerateWidgets();
};

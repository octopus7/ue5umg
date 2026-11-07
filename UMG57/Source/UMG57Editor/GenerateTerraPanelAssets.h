// Model: GPT-6.1 Terra; reasoning: Medium
#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GenerateTerraPanelAssets.generated.h"
UCLASS()
class UGenerateTerraPanelAssets : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="Setup") static bool GenerateWidgets();
};

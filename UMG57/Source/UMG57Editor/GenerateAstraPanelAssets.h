#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GenerateAstraPanelAssets.generated.h"
UCLASS()
class UGenerateAstraPanelAssets : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="Setup") static bool GenerateWidgets();
};

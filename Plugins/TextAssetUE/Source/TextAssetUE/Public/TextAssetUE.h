#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TextAssetUE.generated.h"

UCLASS(BlueprintType, HideCategories = (Object))
class TEXTASSETUE_API UTextAssetUE : public UObject {
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, Category = "Text", meta = (MultiLine = true))
  FString Content;
};
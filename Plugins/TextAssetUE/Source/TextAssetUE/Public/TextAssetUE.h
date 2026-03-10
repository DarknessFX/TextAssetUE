#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EditorFramework/AssetImportData.h"
#include "TextAssetUE.generated.h"

USTRUCT(BlueprintType)
struct FTextAssetAppearanceSettings {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, Category = "TextAssetUE|Appearance")
  bool bInheritBackground = true;

  UPROPERTY(EditAnywhere, Category = "TextAssetUE|Appearance", meta = (EditCondition = "!bInheritBackground"))
  FLinearColor BackgroundColor = FLinearColor(0.08f, 0.08f, 0.08f, 1.0f);

  UPROPERTY(EditAnywhere, Category = "TextAssetUE|Appearance")
  bool bInheritTextColor = true;

  UPROPERTY(EditAnywhere, Category = "TextAssetUE|Appearance", meta = (EditCondition = "!bInheritTextColor"))
  FLinearColor TextColor = FLinearColor::White;

  UPROPERTY(EditAnywhere, Category = "TextAssetUE|Appearance", meta = (ClampMin = "8", ClampMax = "64"))
  int32 FontSize = 14;

  UPROPERTY(EditAnywhere, Category = "TextAssetUE|Appearance", meta = (AllowedClasses = "/Script/Engine.Font"))
  TSoftObjectPtr<UFont> FontAsset;

  UPROPERTY(EditAnywhere, Category = "TextAssetUE|Appearance")
  bool bWordWrap = true;

  UPROPERTY(EditAnywhere, Category = "TextAssetUE|Appearance")
  FMargin TextMargin = FMargin(4.0f);
};

UCLASS(BlueprintType, HideCategories = (Object))
class TEXTASSETUE_API UTextAssetUE : public UObject {
  GENERATED_BODY()

public:
  UTextAssetUE();

  UPROPERTY(EditAnywhere, Category = "TextAssetUE|Text", meta = (MultiLine = true))
  FText Content;

  UPROPERTY(VisibleAnywhere, Instanced, Category = "TextAssetUE|ImportSettings", meta = (EditCondition = "false"))
  UAssetImportData* AssetImportData;

  UPROPERTY(EditAnywhere, Category = "TextAssetUE|Appearance")
  FTextAssetAppearanceSettings Appearance;
};
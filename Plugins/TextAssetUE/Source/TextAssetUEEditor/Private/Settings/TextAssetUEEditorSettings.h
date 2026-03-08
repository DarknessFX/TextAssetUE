
#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "Styling/SlateColor.h"
#include "TextAssetUEEditorSettings.generated.h"

UCLASS(config = Editor, defaultconfig)
class TEXTASSETUEEDITOR_API UTextAssetUEEditorSettings : public UObject {
  GENERATED_BODY()

public:
  UPROPERTY(config, EditAnywhere, Category = "Appearance")
  bool bInheritBackground = true;

  UPROPERTY(config, EditAnywhere, Category = "Appearance", meta = (EditCondition="!bInheritBackground"))
  FLinearColor BackgroundColor = FLinearColor(0.1f, 0.1f, 0.1f, 1.0f);

  UPROPERTY(config, EditAnywhere, Category = "Appearance")
  bool bInheritTextColor = true;

  UPROPERTY(config, EditAnywhere, Category = "Appearance", meta = (EditCondition="!bInheritTextColor"))
  FLinearColor TextColor = FLinearColor::White;

  UPROPERTY(config, EditAnywhere, Category = "Appearance", meta = (ClampMin = "8", ClampMax = "64"))
  int32 FontSize = 14;

  UPROPERTY(config, EditAnywhere, Category = "Appearance", meta = (AllowedClasses = "/Script/Engine.Font", DisplayName = "Font Family"))
  TSoftObjectPtr<UFont> FontAsset;

  UPROPERTY(config, EditAnywhere, Category = "Appearance")
  bool bWordWrap = true;

  UPROPERTY(config, EditAnywhere, Category = "Appearance")
  FMargin TextMargin = FMargin(4.0f);
};
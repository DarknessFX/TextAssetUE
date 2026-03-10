#include "TextAssetUE.h"

UTextAssetUE::UTextAssetUE() {
  FString Section = TEXT("/Script/TextAssetUE.TextAssetUEEditorSettings");

  bool bInheritBackground = true;
  GConfig->GetBool(*Section, TEXT("bInheritBackground"), bInheritBackground, GEditorPerProjectIni);
  Appearance.bInheritBackground = bInheritBackground;

  FColor BackgroundColorInt;
  if (!GConfig->GetColor(*Section, TEXT("BackgroundColor"), BackgroundColorInt, GEditorPerProjectIni)) {
    BackgroundColorInt = FColor(20, 20, 20, 255);
  }
  Appearance.BackgroundColor = BackgroundColorInt.ReinterpretAsLinear();

  bool bInheritTextColor = true;
  GConfig->GetBool(*Section, TEXT("bInheritTextColor"), bInheritTextColor, GEditorPerProjectIni);
  Appearance.bInheritTextColor = bInheritTextColor;

  FColor TextColorInt;
  if (!GConfig->GetColor(*Section, TEXT("TextColor"), TextColorInt, GEditorPerProjectIni)) {
    TextColorInt = FColor::White;
  }
  Appearance.TextColor = TextColorInt.ReinterpretAsLinear();

  int32 FontSize;
  if (!GConfig->GetInt(*Section, TEXT("FontSize"), FontSize, GEditorPerProjectIni)) {
    FontSize = 14;
  }
  Appearance.FontSize = FontSize;

  FString FontPath;
  if (GConfig->GetString(*Section, TEXT("FontAsset"), FontPath, GEditorPerProjectIni)) {
    Appearance.FontAsset = TSoftObjectPtr<UFont>(FSoftObjectPath(FontPath));
  } else {
    Appearance.FontAsset = nullptr;
  }

  bool bWordWrap;
  if (!GConfig->GetBool(*Section, TEXT("bWordWrap"), bWordWrap, GEditorPerProjectIni)) {
    bWordWrap = true;
  }
  Appearance.bWordWrap = bWordWrap;

  FVector MarginVec(4.f, 4.f, 4.f);
  if (GConfig->GetVector(*Section, TEXT("TextMargin"), MarginVec, GEditorPerProjectIni)) {
    Appearance.TextMargin = FMargin(MarginVec.X, MarginVec.Y, MarginVec.Z, MarginVec.Z); // simple approximation
  } else {
    Appearance.TextMargin = FMargin(4.f);
  }
}
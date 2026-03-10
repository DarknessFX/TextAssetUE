#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "TextAssetUE.h"
#include "TextAssetUE_BP.generated.h"

UCLASS()
class TEXTASSETUE_API UTextAssetUE_BP : public UBlueprintFunctionLibrary {
  GENERATED_BODY()

public:

  UFUNCTION(BlueprintPure, Category="TextAssetUE|Text", meta = (DevelopmentOnly))
  static FText GetText(const UTextAssetUE* Asset);

  UFUNCTION(BlueprintCallable, Category="TextAssetUE|Text", meta = (DevelopmentOnly))
  static void SetText(UTextAssetUE* Asset, const FText& NewText);

  UFUNCTION(BlueprintCallable, Category = "TextAssetUE|Text", meta = (DevelopmentOnly))
  static void AppendText(UTextAssetUE* Asset, const FText& NewLine, bool bTimestamp = false);

  UFUNCTION(BlueprintCallable, Category = "TextAssetUE|Text", meta = (DevelopmentOnly))
  static int32 GetLineCount(UTextAssetUE* Asset);

  UFUNCTION(BlueprintCallable, Category = "TextAssetUE|Text", meta = (DevelopmentOnly))
  static bool GetLine(UTextAssetUE* Asset, int32 LineIndex, FText& OutLine);

  UFUNCTION(BlueprintCallable, Category = "TextAssetUE|Text", meta = (DevelopmentOnly))
  static bool RemoveLine(UTextAssetUE* Asset, int32 LineIndex);

  UFUNCTION(BlueprintCallable, Category = "TextAssetUE|Text", meta = (DevelopmentOnly))
  static bool ReplaceLine(UTextAssetUE* Asset, int32 LineIndex, const FText& NewLine);

  UFUNCTION(BlueprintCallable, Category = "TextAssetUE|Text", meta = (DevelopmentOnly))
  static void AppendLines(UTextAssetUE* Asset, const TArray<FText>& Lines, bool bTimestamp);

  UFUNCTION(BlueprintCallable, Category = "TextAssetUE|Text", meta = (DevelopmentOnly))
  static void FindLinesContaining(UTextAssetUE* Asset, const FString& Substring, TArray<int32>& OutLineIndices);

  UFUNCTION(BlueprintCallable, Category = "TextAssetUE|Text", meta = (DevelopmentOnly))
  static void ExportToString(UTextAssetUE* Asset, FString& OutString);

  UFUNCTION(BlueprintCallable, Category = "TextAssetUE|Text", meta = (DevelopmentOnly))
  static void ImportFromString(UTextAssetUE* Asset, const FString& Text);
};
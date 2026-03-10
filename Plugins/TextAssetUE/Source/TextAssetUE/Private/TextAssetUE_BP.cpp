#include "TextAssetUE_BP.h"
#include "TextAssetUEModule.h"
#include "EngineGlobals.h"
#include "Misc/DateTime.h"
#include "AssetToolsModule.h"
#include "IAssetTools.h"

FText UTextAssetUE_BP::GetText(const UTextAssetUE* Asset) {
  if (!Asset) { return FText::GetEmpty(); }
  return Asset->Content;
}

void UTextAssetUE_BP::SetText(UTextAssetUE* Asset, const FText& NewText) {
  if (!Asset) { return; }
  Asset->Content = NewText;
}

void UTextAssetUE_BP::AppendText(UTextAssetUE* Asset, const FText& NewLine, bool bTimestamp) {
  if (!Asset) { return; }

  FString Line = NewLine.ToString();
  if (bTimestamp) {
    FDateTime Now = FDateTime::Now();
    FString Timestamp = FString::Printf(TEXT("[%04d.%02d.%02d-%02d.%02d.%02d:%03d][%llu]"),
      Now.GetYear(), Now.GetMonth(), Now.GetDay(),
      Now.GetHour(), Now.GetMinute(), Now.GetSecond(),
      Now.GetMillisecond(), GFrameCounter);
    Line = Timestamp + Line;
  }

  FString Current = Asset->Content.ToString();
  if (!Current.IsEmpty() && !Current.EndsWith(LINE_TERMINATOR)) {
    Current += LINE_TERMINATOR;
  }

  Asset->Content = FText::FromString(Current + Line + LINE_TERMINATOR);
}

int32 UTextAssetUE_BP::GetLineCount(UTextAssetUE* Asset) {
  if (!Asset) { return 0; }

  TArray<FString> Lines;
  Asset->Content.ToString().ParseIntoArrayLines(Lines);
  return Lines.Num();
}

bool UTextAssetUE_BP::GetLine(UTextAssetUE* Asset, int32 LineIndex, FText& OutLine) {
  if (!Asset) { return false; }

  TArray<FString> Lines;
  Asset->Content.ToString().ParseIntoArrayLines(Lines);
  if (!Lines.IsValidIndex(LineIndex)) { return false; }

  OutLine = FText::FromString(Lines[LineIndex]);
  return true;
}

bool UTextAssetUE_BP::RemoveLine(UTextAssetUE* Asset, int32 LineIndex) {
  if (!Asset) { return false; }

  TArray<FString> Lines;
  Asset->Content.ToString().ParseIntoArrayLines(Lines);
  if (!Lines.IsValidIndex(LineIndex)) { return false; }

  Lines.RemoveAt(LineIndex);
  Asset->Content = FText::FromString(FString::Join(Lines, TEXT("\n")));
  return true;
}

bool UTextAssetUE_BP::ReplaceLine(UTextAssetUE* Asset, int32 LineIndex, const FText& NewLine) {
  if (!Asset) { return false; }

  TArray<FString> Lines;
  Asset->Content.ToString().ParseIntoArrayLines(Lines);
  if (!Lines.IsValidIndex(LineIndex)) { return false; }

  Lines[LineIndex] = NewLine.ToString();
  Asset->Content = FText::FromString(FString::Join(Lines, TEXT("\n")));
  return true;
}

void UTextAssetUE_BP::AppendLines(UTextAssetUE* Asset, const TArray<FText>& Lines, bool bTimestamp) {
  for (const FText& Line : Lines) {
    AppendText(Asset, Line, bTimestamp);
  }
}

void UTextAssetUE_BP::ExportToString(UTextAssetUE* Asset, FString& OutString) {
  OutString = Asset ? Asset->Content.ToString() : FString();
}

void UTextAssetUE_BP::ImportFromString(UTextAssetUE* Asset, const FString& Text) {
  if (!Asset) { return; }
  Asset->Content = FText::FromString(Text);
}

void UTextAssetUE_BP::FindLinesContaining(UTextAssetUE* Asset, const FString& Substring, TArray<int32>& OutLineIndices) {
  OutLineIndices.Reset();
  if (!Asset) { return; }

  TArray<FString> Lines;
  Asset->Content.ToString().ParseIntoArrayLines(Lines);

  for (int32 i = 0; i < Lines.Num(); i++) {
    if (Lines[i].Contains(Substring)) {
      OutLineIndices.Add(i);
    }
  }
}
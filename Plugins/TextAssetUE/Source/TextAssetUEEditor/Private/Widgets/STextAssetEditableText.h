#pragma once

#include "CoreMinimal.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "ScopedTransaction.h"

//#include "Widgets/Text/SlateEditableTextLayout.h"
//, public SMultiLineEditableText

class STextAssetEditableText : public SMultiLineEditableTextBox {
public:
  SLATE_BEGIN_ARGS(STextAssetEditableText) {}
    SLATE_ATTRIBUTE(FText, Text)
    SLATE_EVENT(FOnTextChanged, OnTextChanged)
    SLATE_ATTRIBUTE(FSlateColor, BackgroundColor)
    SLATE_ATTRIBUTE(FSlateColor, ForegroundColor)
    SLATE_ATTRIBUTE(FSlateFontInfo, Font)
    SLATE_ARGUMENT(bool, WordWrap)
    SLATE_ATTRIBUTE(FMargin, Margin)
  SLATE_END_ARGS()

  void Construct(const FArguments& InArgs);

  FSlateFontInfo CurrentFont;

  FTextRange GetSelectionRange() const;
  void SetSelectionRange(const FTextRange& InRange);
  void SetSelection(int32 BeginIndex, int32 EndIndex);

  FTextLocation GetCursorLocation() const;
  FText GetCursorPositionText() const;
  void SetCursorLocation(const FTextLocation& NewLocation);

  int32 LocationToOffset(const FTextLocation& Location) const;
  FTextLocation OffsetToLocation(int32 AbsoluteOffset) const;

  FSlateFontInfo GetFont() const;
  void SetFont(const FSlateFontInfo& NewFont);
  void SetFontSize(int32 NewSize);
  void SetFontObject(UObject* NewFont);

};
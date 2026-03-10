#include "STextAssetEditableText.h"

#define LOCTEXT_NAMESPACE "TextAssetUEEditor"

void STextAssetEditableText::Construct(const FArguments& InArgs) {
  CurrentFont = InArgs._Font.Get();

  //FEditableTextBoxStyle NewStyle = Editor->Style();
  //NewStyle.BackgroundImageNormal = FCoreStyle::Get().GetBrush("WhiteBrush");
  //NewStyle.BackgroundImageHovered = FCoreStyle::Get().GetBrush("WhiteBrush");
  //NewStyle.BackgroundImageFocused = FCoreStyle::Get().GetBrush("WhiteBrush");
  //MyEditableTextBox->SetStyle(NewStyle);
  //MyEditableTextBox->SetBackgroundColor(MyLinearColor);

  SMultiLineEditableTextBox::Construct(
    SMultiLineEditableTextBox::FArguments()
    .Text(InArgs._Text)
    .OnTextChanged(InArgs._OnTextChanged)
    .AllowMultiLine(true)
    .IsReadOnly(false)
    .Font_Lambda([this]() { return CurrentFont; })
    .Margin(InArgs._Margin)
    .Style(&FEditableTextBoxStyle::GetDefault())
    .BackgroundColor(InArgs._BackgroundColor)
    .ForegroundColor(InArgs._ForegroundColor)
    .AutoWrapText(InArgs._WordWrap)
 );
}

// Selection don't exist on SMultiLineEditableTextBox
// all Selection features are broken since changed 
// from SMultiLineEditableText / SlateEditableTextLayout
FTextRange STextAssetEditableText::GetSelectionRange() const {
  //if (this) {
  //  return SMultiLineEditableTextBox::GetSelectionRange();
  //}
  return FTextRange();
}

void STextAssetEditableText::SetSelectionRange(const FTextRange& InRange) {
  //if (this) {
  //  SMultiLineEditableTextBox::SetSelection(InRange.BeginIndex, InRange.EndIndex);
  //}
}

void STextAssetEditableText::SetSelection(int32 BeginIndex, int32 EndIndex) {
  //if (this) {
  //  SMultiLineEditableTextBox::SetSelection(BeginIndex, EndIndex);
  //}
}

FTextLocation STextAssetEditableText::GetCursorLocation() const {
  if (this) {
    return SMultiLineEditableTextBox::GetCursorLocation();
  }
  return FTextLocation();
}

void STextAssetEditableText::SetCursorLocation(const FTextLocation& NewLocation) {
  if (this) {
    GoTo(NewLocation);
  }
}

int32 STextAssetEditableText::LocationToOffset(const FTextLocation& Location) const {
  if (!this) {
    return 0;
  }

  FString FullText = GetText().ToString();
  int32 Offset = 0;
  int32 CurrentLine = 0;

  for (TCHAR Ch : FullText) {
    if (CurrentLine == Location.GetLineIndex()) {
      Offset += Location.GetOffset();
      return Offset;
    }

    Offset++;
    if (Ch == '\n') {
      CurrentLine++;
    }
  }

  return FullText.Len();
}

FTextLocation STextAssetEditableText::OffsetToLocation(int32 AbsoluteOffset) const {
  if (!this || AbsoluteOffset <= 0) {
    return FTextLocation();
  }

  FString FullText = GetText().ToString();
  int32 CurrentOffset = 0;
  int32 Line = 0;
  int32 LineOffset = 0;

  for (int32 i = 0; i < FullText.Len(); ++i) {
    if (CurrentOffset == AbsoluteOffset) {
      return FTextLocation(Line, LineOffset);
    }

    if (FullText[i] == '\n') {
      Line++;
      LineOffset = 0;
    }
    else {
      LineOffset++;
    }
    CurrentOffset++;
  }

  return FTextLocation(Line, LineOffset);
}

FText STextAssetEditableText::GetCursorPositionText() const {
  FTextLocation Cursor = SMultiLineEditableTextBox::GetCursorLocation();
  return FText::Format(LOCTEXT("CursorPosFmt", "Ln {0}, Col {1}"),
    FText::AsNumber(Cursor.GetLineIndex() + 1),
    FText::AsNumber(Cursor.GetOffset() + 1)
  );
}

void STextAssetEditableText::SetFontSize(int32 NewSize) {
  CurrentFont.Size = NewSize;
}

void STextAssetEditableText::SetFontObject(UObject* NewFont) {
  CurrentFont.FontObject = NewFont;
}

FSlateFontInfo STextAssetEditableText::GetFont() const {
  return CurrentFont; 
}

void STextAssetEditableText::SetFont(const FSlateFontInfo& NewFont) {
  CurrentFont = NewFont;
}

#undef LOCTEXT_NAMESPACE
#include "SFindReplaceDialog.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Layout/SUniformGridPanel.h"

#define LOCTEXT_NAMESPACE "TextAssetUEEditor"

void SFindReplaceDialog::Construct(const FArguments& InArgs, TWeakPtr<FTextAssetUEEditorToolkit> InToolkit) {
  OwningToolkit = InToolkit;

  ChildSlot
  [
    SNew(SBorder)
      .BorderImage(FAppStyle::GetBrush("Docking.Background"))
      .Padding(8.0f)
      [
        SNew(SVerticalBox)
          + SVerticalBox::Slot()
          .AutoHeight()
          .Padding(0, 0, 0, 4)
          [
            SNew(SHorizontalBox)
              + SHorizontalBox::Slot()
              .AutoWidth()
              .Padding(0, 4, 8, 4)
              [
                SNew(STextBlock).Text(LOCTEXT("FindLabel", "Find:"))
                  .MinDesiredWidth(80.0f)
              ]
              + SHorizontalBox::Slot()
              .FillWidth(1.0)
              [
                SAssignNew(FindTextBox, SEditableTextBox)
                  .HintText(LOCTEXT("FindHint", "Find ..."))
              ]
          ]
        + SVerticalBox::Slot()
          .AutoHeight()
          .Padding(0, 0, 0, 8)
          [
            SNew(SHorizontalBox)
              + SHorizontalBox::Slot()
              .AutoWidth()
              .Padding(0, 4, 8, 4)
              [
                SNew(STextBlock).Text(LOCTEXT("ReplaceLabel", "Replace:"))
                  .MinDesiredWidth(80.0f)
              ]
              + SHorizontalBox::Slot().AutoWidth()
              .FillWidth(1.0)
              [
                SAssignNew(ReplaceTextBox, SEditableTextBox)
                  .HintText(LOCTEXT("ReplaceHint", "Replace ..."))
              ]
          ]
        + SVerticalBox::Slot()
          .AutoHeight()
          .Padding(0, 0, 0, 12)
          [
            SNew(SHorizontalBox)
              + SHorizontalBox::Slot()
              .AutoWidth()
              [
                SAssignNew(CaseSensitiveCheck, SCheckBox)
                  .IsChecked(ECheckBoxState::Unchecked)
              ]
              + SHorizontalBox::Slot()
              .AutoWidth()
              .Padding(4, 0, 12, 0)
              [
                SNew(STextBlock).Text(LOCTEXT("CaseSensitive", "Case sensitive"))
              ]
          ]
        + SVerticalBox::Slot()
          .HAlign(EHorizontalAlignment::HAlign_Right)
          .AutoHeight()
          [
            SNew(SHorizontalBox)
              + SHorizontalBox::Slot()
              .AutoWidth()
              .Padding(8, 0, 0, 0)
              [
                SNew(SButton)
                  .Text(LOCTEXT("FindNext", "Find Next"))
                  .OnClicked(this, &SFindReplaceDialog::OnFindNextClicked)
              ]
              + SHorizontalBox::Slot()
              .AutoWidth()
              .Padding(8, 0, 0, 0)
              [
                SNew(SButton)
                  .Text(LOCTEXT("Replace", "Replace"))
                  .OnClicked(this, &SFindReplaceDialog::OnReplaceClicked)
              ]
              + SHorizontalBox::Slot()
              .AutoWidth()
              .Padding(8, 0, 0, 0)
              [
                SNew(SButton)
                  .Text(LOCTEXT("ReplaceAll", "Replace All"))
                  .OnClicked(this, &SFindReplaceDialog::OnReplaceAllClicked)
              ]
              + SHorizontalBox::Slot()
              .AutoWidth()
              .Padding(8, 0, 0, 0)
              [
                SNew(SButton)
                  .Text(LOCTEXT("Close", "Close"))
                  .OnClicked(this, &SFindReplaceDialog::OnCloseClicked)
              ]
          ]
      ]
  ];
}

FReply SFindReplaceDialog::OnFindNextClicked() {
  if (OwningToolkit.IsValid()) {
    OwningToolkit.Pin()->CurrentFindText = FindTextBox->GetText();
    OwningToolkit.Pin()->bCaseSensitive = CaseSensitiveCheck->IsChecked();
    OwningToolkit.Pin()->PerformFindNext();
  }
  return FReply::Handled();
}

FReply SFindReplaceDialog::OnReplaceClicked() {
  if (OwningToolkit.IsValid()) {
    OwningToolkit.Pin()->CurrentFindText = FindTextBox->GetText();
    OwningToolkit.Pin()->CurrentReplaceText = ReplaceTextBox->GetText();
    OwningToolkit.Pin()->bCaseSensitive = CaseSensitiveCheck->IsChecked();
    OwningToolkit.Pin()->PerformReplace();
  }
  return FReply::Handled();
}

FReply SFindReplaceDialog::OnReplaceAllClicked() {
  if (OwningToolkit.IsValid()) {
    OwningToolkit.Pin()->CurrentFindText = FindTextBox->GetText();
    OwningToolkit.Pin()->CurrentReplaceText = ReplaceTextBox->GetText();
    OwningToolkit.Pin()->bCaseSensitive = CaseSensitiveCheck->IsChecked();
    OwningToolkit.Pin()->PerformReplaceAll();
  }
  return FReply::Handled();
}

FReply SFindReplaceDialog::OnCloseClicked() {
  if (OwningToolkit.IsValid()) {
    OwningToolkit.Pin()->CloseFindReplaceDialog();
  }
  return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
#include "SAppearanceDialog.h"
#include "Engine/Font.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Toolkits/TextAssetUEEditorToolkit.h"

#define LOCTEXT_NAMESPACE "TextAssetUEEditor"

void SAppearanceDialog::Construct(const FArguments& InArgs) {
  EditingAsset = InArgs._EditingAsset;
  TextEditorPtr = InArgs._TextEditorPtr;
  OwningToolkit = InArgs._OwningToolkit;

  BackgroundColor = EditingAsset->Appearance.BackgroundColor;
  TextColor = EditingAsset->Appearance.TextColor;

  if (EditingAsset.IsValid() && EditingAsset->Appearance.FontAsset.IsValid()) {
    SelectedFontPtr = EditingAsset->Appearance.FontAsset.Get();
  } else {
    SelectedFontPtr = nullptr;
  }

  //UE_LOG(LogTemp, Warning, TEXT("FontSize %d"), EditingAsset.IsValid() ? EditingAsset.Get()->Appearance.FontSize : 14);

  ChildSlot
    [
      SNew(SBorder)
        .Padding(8)
        [
          SNew(SVerticalBox)

            + SVerticalBox::Slot()
            .AutoHeight()
            [
              SNew(SGridPanel)
                .FillColumn(1, 1.0f)

              + SGridPanel::Slot(0, 0)
                .VAlign(VAlign_Center)
                .Padding(0, 2)
                [
                  SNew(STextBlock).Text(LOCTEXT("Background", "Background"))
                ]
                + SGridPanel::Slot(1, 0)
                .Padding(4, 2)
                [
                  SNew(SHorizontalBox)
                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    [
                      SAssignNew(InheritBackgroundCheckBox, SCheckBox)
                        .IsChecked(EditingAsset.IsValid()
                          ? (EditingAsset.Get()->Appearance.bInheritBackground ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
                          : ECheckBoxState::Unchecked)
                    ]
                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    .Padding(4, 0, 0, 0)
                    .VAlign(VAlign_Center)
                    [
                      SNew(STextBlock).Text(LOCTEXT("InheritBackground", "Inherit"))
                    ]
                    + SHorizontalBox::Slot()
                    .FillWidth(1.0f)
                    .Padding(4, 0, 0, 0)
                    [
                      SAssignNew(BackgroundColorBlock, SColorBlock)
                        .Color_Lambda([this]() { return BackgroundColor; })
                        .IsEnabled_Lambda([this]() { return !InheritBackgroundCheckBox->IsChecked(); })
                        .OnMouseButtonDown(this, &SAppearanceDialog::OnBackgroundColorClicked)
                    ]
                ]

              + SGridPanel::Slot(0, 1)
                .VAlign(VAlign_Center)
                .Padding(0, 2)
                [
                  SNew(STextBlock).Text(LOCTEXT("TextColor", "Text Color"))
                ]
                + SGridPanel::Slot(1, 1)
                .Padding(4, 2)
                [
                  SNew(SHorizontalBox)
                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    [
                      SAssignNew(InheritTextColorCheckBox, SCheckBox)
                        .IsChecked(EditingAsset.IsValid()
                          ? (EditingAsset.Get()->Appearance.bInheritTextColor ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
                          : ECheckBoxState::Unchecked)
                    ]
                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    .Padding(4, 0, 0, 0)
                    .VAlign(VAlign_Center)
                    [
                      SNew(STextBlock).Text(LOCTEXT("InheritTextColor", "Inherit"))
                    ]
                    + SHorizontalBox::Slot()
                    .FillWidth(1.0f)
                    .Padding(4, 0, 0, 0)
                    [
                      SAssignNew(TextColorBlock, SColorBlock)
                        .Color_Lambda([this]() { return TextColor; })
                        .IsEnabled_Lambda([this]() { return !InheritTextColorCheckBox->IsChecked(); })
                        .OnMouseButtonDown(this, &SAppearanceDialog::OnTextColorClicked)
                    ]
                ]

              + SGridPanel::Slot(0, 2)
                .VAlign(VAlign_Center)
                .Padding(0, 2)
                [
                  SNew(STextBlock).Text(LOCTEXT("FontSizeLabel", "Font Size"))
                ]
                + SGridPanel::Slot(1, 2)
                .Padding(4, 2)
                [
                  SAssignNew(FontSizeTextBox, SEditableTextBox)
                    .Text(FText::AsNumber(EditingAsset.IsValid() ? EditingAsset.Get()->Appearance.FontSize : 14))
                ]

              + SGridPanel::Slot(0, 3)
                .VAlign(VAlign_Center)
                .Padding(0, 2)
                [
                  SNew(STextBlock).Text(LOCTEXT("FontAsset", "Font"))
                ]
                + SGridPanel::Slot(1, 3)
                .Padding(4, 2)
                [
                  SAssignNew(FontAssetPicker, SObjectPropertyEntryBox)
                    .AllowedClass(UFont::StaticClass())
                    .ObjectPath_Lambda([this]() -> FString {
                      return SelectedFontPtr.IsValid()
                        ? SelectedFontPtr->GetPathName()
                        : FString();
                    })
                    .OnObjectChanged_Lambda([this](const FAssetData& AssetData) {
                      SelectedFontPtr = Cast<UFont>(AssetData.GetAsset());
                      if (EditingAsset.IsValid()) {
                        EditingAsset->Appearance.FontAsset = SelectedFontPtr.Get();
                      }
                    })
                ]

              + SGridPanel::Slot(0, 4)
                .VAlign(VAlign_Center)
                .Padding(0, 2)
                [
                  SNew(STextBlock).Text(LOCTEXT("WordWrap", "Word Wrap"))
                ]
                + SGridPanel::Slot(1, 4)
                .Padding(4, 2)
                [
                  SAssignNew(WordWrapCheckBox, SCheckBox)
                    .IsChecked(EditingAsset.IsValid()
                      ? (EditingAsset.Get()->Appearance.bWordWrap ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
                      : ECheckBoxState::Unchecked)
                ]

              + SGridPanel::Slot(0, 5)
                .VAlign(VAlign_Center)
                .Padding(0, 2)
                [
                  SNew(STextBlock).Text(LOCTEXT("Margin", "Text Margin"))
                ]
                + SGridPanel::Slot(1, 5)
                .Padding(4, 2)
                [
                  SAssignNew(MarginTextBox, SEditableTextBox)
                    .Text(FText::AsNumber(
                      EditingAsset.IsValid()
                      ? EditingAsset.Get()->Appearance.TextMargin.Left
                      : 4.0f
                    ))
                    .HintText(LOCTEXT("MarginHint", "All sides..."))
                ]
            ]
          + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(4, 2)
            [
              SNew(STextBlock)
                .Text(LOCTEXT("FontChangeNotice", "Font asset changes require saving the TextAsset, closing the TextAsset editor tab and reopening it."))
                .ColorAndOpacity(FLinearColor::Yellow)
                .Font(FAppStyle::GetFontStyle("NormalFont"))
                .AutoWrapText(true)
            ]

          + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Right)
            .Padding(0, 10, 0, 0)
            [
              SNew(SHorizontalBox)
                + SHorizontalBox::Slot()
                .AutoWidth()
                [
                  SNew(SButton)
                    .Text(LOCTEXT("Apply", "Apply"))
                    .OnClicked(this, &SAppearanceDialog::OnApplyClicked)
                ]
                + SHorizontalBox::Slot()
                .AutoWidth()
                .Padding(8, 0)
                [
                  SNew(SButton)
                    .Text(LOCTEXT("Close", "Close"))
                    .OnClicked(this, &SAppearanceDialog::OnCloseClicked)
                ]
            ]
        ]
    ];
}

FReply SAppearanceDialog::OnApplyClicked() {
  if (!EditingAsset.IsValid() || !TextEditorPtr.IsValid()) {
    return FReply::Handled();
  }

  TSharedPtr<STextAssetEditableText> Editor = TextEditorPtr.Pin();
  UTextAssetUE* _EditingAsset = EditingAsset.Get();
  FSlateFontInfo FontInfo = Editor->GetFont();

  _EditingAsset->Modify();

  _EditingAsset->Appearance.bInheritBackground = InheritBackgroundCheckBox->IsChecked();
  if (!_EditingAsset->Appearance.bInheritBackground && BackgroundColorBlock.IsValid()) {
    _EditingAsset->Appearance.BackgroundColor = BackgroundColor;
    Editor->SetTextBoxBackgroundColor(BackgroundColor);
  }

  _EditingAsset->Appearance.bInheritTextColor = InheritTextColorCheckBox->IsChecked();
  if (!_EditingAsset->Appearance.bInheritTextColor && TextColorBlock.IsValid()) {
    _EditingAsset->Appearance.TextColor = TextColor;
    Editor->SetTextBoxForegroundColor(TextColor);
  }

  if (SelectedFontPtr.IsValid()) {
    _EditingAsset->Appearance.FontAsset = SelectedFontPtr.Get();
    FontInfo.FontObject = SelectedFontPtr.Get();
  } else {
    _EditingAsset->Appearance.FontAsset = nullptr;
    FontInfo.FontObject = nullptr;
  }

  int32 NewFontSize = FCString::Atoi(*FontSizeTextBox->GetText().ToString());
  _EditingAsset->Appearance.FontSize = NewFontSize;
  FontInfo.Size = NewFontSize;

  Editor->SetFont(FontInfo);

  _EditingAsset->Appearance.bWordWrap = WordWrapCheckBox->IsChecked();
  Editor->SetAutoWrapText(WordWrapCheckBox->IsChecked());

  if (MarginTextBox.IsValid()) {
    float NewMargin = FCString::Atof(*MarginTextBox->GetText().ToString());
    _EditingAsset->Appearance.TextMargin = FMargin(NewMargin);
    Editor->SetMargin(_EditingAsset->Appearance.TextMargin);
  }

  return FReply::Handled();
}

FReply SAppearanceDialog::OnCloseClicked() {
  if (OwningToolkit.IsValid()) {
    OwningToolkit.Get()->CloseAppearanceDialog();
  }
  return FReply::Handled();
}

FReply SAppearanceDialog::OnBackgroundColorClicked(const FGeometry&, const FPointerEvent&) {
  FColorPickerArgs Args;
  Args.InitialColor = BackgroundColor;
  Args.bUseAlpha = true;
  Args.bIsModal = false;
  Args.OnColorCommitted = FOnLinearColorValueChanged::CreateLambda(
    [this](FLinearColor NewColor) {
      BackgroundColor = NewColor;
      if (BackgroundColorBlock.IsValid()) {
        BackgroundColorBlock->Invalidate(EInvalidateWidget::Paint);
      }
    });

  OpenColorPicker(Args);
  return FReply::Handled();
}

FReply SAppearanceDialog::OnTextColorClicked(const FGeometry&, const FPointerEvent&) {
  FColorPickerArgs Args;
  Args.InitialColor = TextColor;
  Args.bUseAlpha = true;
  Args.bIsModal = false;
  Args.OnColorCommitted = FOnLinearColorValueChanged::CreateLambda(
    [this](FLinearColor NewColor) {
      TextColor = NewColor;
      if (TextColorBlock.IsValid()) {
        TextColorBlock->Invalidate(EInvalidateWidget::Paint);
      }
    });

  OpenColorPicker(Args);
  return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
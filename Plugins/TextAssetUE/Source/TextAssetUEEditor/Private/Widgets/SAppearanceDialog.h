#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Colors/SColorPicker.h"
#include "Widgets/Colors/SColorBlock.h"
#include "PropertyCustomizationHelpers.h"

class UTextAssetUE;
class STextAssetEditableText;
class FTextAssetUEEditorToolkit;

class SAppearanceDialog : public SCompoundWidget {
public:
  SLATE_BEGIN_ARGS(SAppearanceDialog) {}
    SLATE_ARGUMENT(UTextAssetUE*, EditingAsset)
    SLATE_ARGUMENT(TWeakPtr<STextAssetEditableText>, TextEditorPtr)
    SLATE_ARGUMENT(TSharedPtr<FTextAssetUEEditorToolkit>, OwningToolkit)
  SLATE_END_ARGS()

  void Construct(const FArguments& InArgs);

  FReply OnApplyClicked();
  FReply OnCloseClicked();

  FReply OnBackgroundColorClicked(const FGeometry&, const FPointerEvent&);
  FReply OnTextColorClicked(const FGeometry&, const FPointerEvent&);

  TWeakObjectPtr<UTextAssetUE> EditingAsset;
  TWeakPtr<STextAssetEditableText> TextEditorPtr;
  TSharedPtr<FTextAssetUEEditorToolkit> OwningToolkit;

  TSharedPtr<SCheckBox> InheritBackgroundCheckBox;
  FLinearColor BackgroundColor;
  TSharedPtr<SColorBlock> BackgroundColorBlock;
  TSharedPtr<SCheckBox> InheritTextColorCheckBox;
  FLinearColor TextColor;
  TSharedPtr<SColorBlock> TextColorBlock;
  TSharedPtr<SEditableTextBox> FontSizeTextBox;
  TSharedPtr<SObjectPropertyEntryBox> FontAssetPicker;
  TWeakObjectPtr<UFont> SelectedFontPtr;
  TSharedPtr<SCheckBox> WordWrapCheckBox;
  TSharedPtr<SEditableTextBox> MarginTextBox;
};
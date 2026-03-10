#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SCheckBox.h"
#include "Toolkits/TextAssetUEEditorToolkit.h"

class SFindReplaceDialog : public SCompoundWidget {
public:
  SLATE_BEGIN_ARGS(SFindReplaceDialog) {}
  SLATE_END_ARGS()

  void Construct(const FArguments& InArgs, TWeakPtr<class FTextAssetUEEditorToolkit> InToolkit);

  TWeakPtr<class FTextAssetUEEditorToolkit> OwningToolkit;
  TSharedPtr<SEditableTextBox> FindTextBox;
  TSharedPtr<SEditableTextBox> ReplaceTextBox;
  TSharedPtr<SCheckBox> CaseSensitiveCheck;

  FReply OnFindNextClicked();
  FReply OnReplaceClicked();
  FReply OnReplaceAllClicked();
  FReply OnCloseClicked();
};
#include "TextAssetUEEditorToolkit.h"
#include "Widgets/Layout/SBorder.h"
#include "Styling/AppStyle.h"
#include "Settings/TextAssetUEEditorSettings.h"
#include "Engine/Font.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Framework/Notifications/NotificationManager.h"
#include "ScopedTransaction.h"
#include "Framework/Commands/GenericCommands.h"
#include "Commands/TextAssetUEEditorCommands.h"
#include "Containers/Ticker.h"

#define LOCTEXT_NAMESPACE "TextAssetUEEditorToolkit"

FTSTicker::FDelegateHandle TickHandle;

void FTextAssetUEEditorToolkit::InitAssetEditor(const EToolkitMode::Type Mode, TSharedPtr<IToolkitHost> InitToolkitHost, FName AppIdentifier, const TSharedRef<FTabManager::FLayout>& InitLayout, bool bCreateDefaultStandaloneTab, bool bUserCreatedDocument, const TArray<UObject*>& ObjectsToEdit) {
  EditingAsset = Cast<UTextAssetUE>(ObjectsToEdit[0]);

  const TSharedRef<FTabManager::FLayout> DefaultLayout = FTabManager::NewLayout("TextAssetUE_Layout_v1")
    ->AddArea(
      FTabManager::NewPrimaryArea()
      ->Split(
        FTabManager::NewStack()
        ->AddTab("TextAssetContentTab", ETabState::OpenedTab)
        ->SetForegroundTab(FTabId("TextAssetContentTab"))
        ->SetHideTabWell(true)
      )
    );

  FTextAssetUEEditorCommands::Register();
  RegisterCommands();
  ExtendToolbar();
  //FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, AppIdentifier, DefaultLayout, bCreateDefaultStandaloneTab, bUserCreatedDocument, ObjectsToEdit);
  FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, AppIdentifier, DefaultLayout, bCreateDefaultStandaloneTab, true, ObjectsToEdit, true, true, TOptional<EAssetOpenMethod>());

  if (TextEditorPtr.IsValid()) {
    FSlateApplication::Get().SetKeyboardFocus(TextEditorPtr.Pin().ToSharedRef(), EFocusCause::SetDirectly);
  }
  FEditorDelegates::PostUndoRedo.AddSP(this, &FTextAssetUEEditorToolkit::RefreshTextWidget);

  TSharedPtr<SDockTab> EditorTab = GetTabManager()->GetOwnerTab();
  if (EditorTab.IsValid()) {
    EditorTab->SetOnTabClosed(SDockTab::FOnTabClosedCallback::CreateLambda([this](TSharedRef<SDockTab> ClosedTab) {
      if (AppearanceDialog.IsValid()) {
        if (TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().FindWidgetWindow(AppearanceDialog.ToSharedRef())) {
          FSlateApplication::Get().RequestDestroyWindow(ParentWindow.ToSharedRef());
        }
        AppearanceDialog.Reset();
      }
    }));
  }

  TickHandle = FTSTicker::GetCoreTicker().AddTicker(
    FTickerDelegate::CreateRaw(this, &FTextAssetUEEditorToolkit::UpdateCursorPositionText), 0.5f
  );
}

void FTextAssetUEEditorToolkit::RefreshTextWidget() {
  if (TextEditorPtr.IsValid() && EditingAsset.IsValid()) {
    TextEditorPtr.Pin()->SetText(EditingAsset->Content);
  }
}

void FTextAssetUEEditorToolkit::RegisterCommands() {
  ToolkitCommands->MapAction(
    FTextAssetUEEditorCommands::Get().Find,
    FExecuteAction::CreateSP(this, &FTextAssetUEEditorToolkit::OpenFindReplaceDialog),
    FCanExecuteAction()
  );

  ToolkitCommands->MapAction(
    FTextAssetUEEditorCommands::Get().Replace,
    FExecuteAction::CreateSP(this, &FTextAssetUEEditorToolkit::OpenFindReplaceDialog),
    FCanExecuteAction()
  );

  ToolkitCommands->MapAction(
    FTextAssetUEEditorCommands::Get().FindNext,
    FExecuteAction::CreateSP(this, &FTextAssetUEEditorToolkit::PerformFindNext),
    FCanExecuteAction()
  );

  //ToolkitCommands->MapAction(
  //  FTextAssetUEEditorCommands::Get().FindPrevious,
  //  FExecuteAction::CreateSP(this, &FTextAssetUEEditorToolkit::PerformFindPrevious),
  //  FCanExecuteAction()
  //);
}

void FTextAssetUEEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) {
  FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

  InTabManager->RegisterTabSpawner("TextAssetContentTab", FOnSpawnTab::CreateSP(this, &FTextAssetUEEditorToolkit::SpawnContentTab))
    .SetDisplayName(NSLOCTEXT("TextAssetUE", "ContentTab", "TextAsset"));
    //.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FTextAssetUEEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) {
  FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
  InTabManager->UnregisterTabSpawner("TextAssetContentTab");

  FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
}

TSharedRef<SDockTab> FTextAssetUEEditorToolkit::SpawnContentTab(const FSpawnTabArgs& Args) {
  return SNew(SDockTab)
    .TabRole(ETabRole::PanelTab)
    [
      CreateInnerWidget()
    ];
}

TSharedRef<SWidget> FTextAssetUEEditorToolkit::CreateInnerWidget() {
  const UTextAssetUEEditorSettings* GlobalSettings = GetDefault<UTextAssetUEEditorSettings>();
  FTextAssetAppearanceSettings Settings;
  Settings.bInheritBackground = GlobalSettings->bInheritBackground;
  Settings.BackgroundColor = GlobalSettings->BackgroundColor;
  Settings.bInheritTextColor = GlobalSettings->bInheritTextColor;
  Settings.TextColor = GlobalSettings->TextColor;
  Settings.FontAsset = GlobalSettings->FontAsset;
  Settings.FontSize = GlobalSettings->FontSize;
  Settings.bWordWrap = GlobalSettings->bWordWrap;
  Settings.TextMargin = GlobalSettings->TextMargin;
  if (EditingAsset.IsValid()) { Settings = EditingAsset->Appearance; }

  FSlateFontInfo FontInfo;
  if (UFont* LoadedFont = Settings.FontAsset.LoadSynchronous()) {
    FontInfo = LoadedFont->GetLegacySlateFontInfo();
  } else {
    FontInfo = FCoreStyle::GetDefaultFontStyle("Regular", Settings.FontSize);
  }
  FontInfo.Size = Settings.FontSize;

  FLinearColor BgColor = Settings.bInheritBackground ? FLinearColor(0.08f, 0.08f, 0.08f, 1.0f) : Settings.BackgroundColor;
  FLinearColor TxtColor = Settings.bInheritTextColor ? FLinearColor::White : Settings.TextColor;

  return SAssignNew(TextEditorPtr, STextAssetEditableText)
        .Text_Lambda([this]() -> FText {
          return EditingAsset.IsValid() ? EditingAsset->Content : FText::GetEmpty();
        })
        .OnTextChanged_Lambda([this](const FText& NewText) {
          if (EditingAsset.IsValid()) {
            EditingAsset->Modify();
            EditingAsset->Content = NewText;
          }
        })
        .Font(FontInfo)
        .Margin(Settings.TextMargin)
        .BackgroundColor(BgColor)
        .ForegroundColor(TxtColor)
        .WordWrap(Settings.bWordWrap);
}

FText FTextAssetUEEditorToolkit::GetLabelForObject(UObject* Object) const {
  if (UTextAssetUE* TextAsset = Cast<UTextAssetUE>(Object)) {
    return FText::FromString(TextAsset->GetName());
  }
  return FText::FromName(Object->GetFName());
}

void FTextAssetUEEditorToolkit::OpenAppearanceDialog() {
  if (!AppearanceDialog.IsValid()) {
    AppearanceDialog = SNew(SAppearanceDialog)
      .EditingAsset(EditingAsset.Get())
      .TextEditorPtr(TextEditorPtr)
      .OwningToolkit(SharedThis(this));
  }

  TSharedRef<SWindow> Window = SNew(SWindow)
    .Title(FText::FromString("Text Asset Appearance"))
    .ClientSize(FVector2D(400, 300))
    .SupportsMaximize(false)
    .SupportsMinimize(false)
    [
      AppearanceDialog.ToSharedRef()
    ];

  FSlateApplication::Get().AddWindow(Window);
}

void FTextAssetUEEditorToolkit::CloseAppearanceDialog() {
  if (AppearanceDialog.IsValid()) {
    TSharedPtr<SWindow> Window = FSlateApplication::Get().FindWidgetWindow(AppearanceDialog.ToSharedRef());
    if (Window.IsValid()) {
      Window->RequestDestroyWindow();
    }
    AppearanceDialog.Reset();
  }
}

void FTextAssetUEEditorToolkit::OpenFindReplaceDialog() {
  if (!FindReplaceDialog.IsValid()) {
    FindReplaceDialog = SNew(SFindReplaceDialog, SharedThis(this));
  }

  if (FindReplaceDialog->FindTextBox.IsValid()) {
    FindReplaceDialog->FindTextBox->SetText(FText::FromString(LastFindText));
  }
  if (FindReplaceDialog->ReplaceTextBox.IsValid()) {
    FindReplaceDialog->ReplaceTextBox->SetText(FText::FromString(LastReplaceText));
  }

  TSharedRef<SWindow> Window = SNew(SWindow)
    .Title(FText::FromString("Find / Replace"))
    .ClientSize(FVector2D(400, 300))
    .SupportsMaximize(false)
    .SupportsMinimize(false)
    [
      FindReplaceDialog.ToSharedRef()
    ];

  FSlateApplication::Get().AddModalWindow(Window, nullptr);
  if (FindReplaceDialog.IsValid() && FindReplaceDialog->FindTextBox.IsValid()) {
    FSlateApplication::Get().SetKeyboardFocus(FindReplaceDialog->FindTextBox.ToSharedRef(), EFocusCause::SetDirectly);
  }
}

void FTextAssetUEEditorToolkit::CloseFindReplaceDialog() {
  if (FindReplaceDialog.IsValid()) {
    TSharedPtr<SWindow> Window = FSlateApplication::Get().FindWidgetWindow(FindReplaceDialog.ToSharedRef());
    if (Window.IsValid()) {
      Window->RequestDestroyWindow();
    }
    FindReplaceDialog.Reset();
  }
}

FText FTextAssetUEEditorToolkit::GetCurrentText() const {
  if (TextEditorPtr.IsValid()) {
    return TextEditorPtr.Pin()->GetText();
  }
  return FText::GetEmpty();
}

void FTextAssetUEEditorToolkit::SetCurrentText(const FText& NewText) {
  if (TextEditorPtr.IsValid()) {
    const FScopedTransaction Transaction(LOCTEXT("EditTextContent", "Edit Text Content"));
    TextEditorPtr.Pin()->SetText(NewText);
  }
}

void FTextAssetUEEditorToolkit::PerformFindNext() {
  if (!TextEditorPtr.IsValid() || CurrentFindText.IsEmpty()) {
    return;
  }

  TSharedPtr<STextAssetEditableText> Editor = TextEditorPtr.Pin();
  FString FindStr = CurrentFindText.ToString();
  FString FullStr = Editor->GetText().ToString();

  LastFindText = FindStr;
  ESearchCase::Type CaseMode = bCaseSensitive ? ESearchCase::CaseSensitive : ESearchCase::IgnoreCase;

  FTextLocation CursorLoc = Editor->GetCursorLocation();
  int32 StartFrom = Editor->LocationToOffset(CursorLoc);

  FTextRange Sel = Editor->GetSelectionRange();
  if (Sel.Len() > 0 && Sel.BeginIndex == StartFrom) {
    StartFrom = Sel.EndIndex;
  }

  int32 FoundAt = FullStr.Find(FindStr, CaseMode, ESearchDir::FromStart, StartFrom);

  if (FoundAt == INDEX_NONE) {
    FoundAt = FullStr.Find(FindStr, CaseMode, ESearchDir::FromStart, 0);
  }

  if (FoundAt != INDEX_NONE) {
    int32 FoundLen = FindStr.Len();
    int32 SafeEnd = FMath::Min(FoundAt + FoundLen, FullStr.Len());

    FTextLocation NewCursor = Editor->OffsetToLocation(FoundAt);
    Editor->SetCursorLocation(NewCursor);
    Editor->SetSelection(FoundAt, SafeEnd);
    Editor->ScrollTo(FoundAt);

    CloseFindReplaceDialog();
    //UE_LOG(LogTemp, Warning, TEXT("Cursor moved to %d, selected %d to %d"), FoundAt, FoundAt, SafeEnd);
  } else {
    FNotificationInfo Info(FText::Format(LOCTEXT("NotFoundFmt", "'{0}' not found"), CurrentFindText));
    Info.ExpireDuration = 3.0f;
    FSlateNotificationManager::Get().AddNotification(Info);
  }
}

bool FTextAssetUEEditorToolkit::PerformReplace(bool bEnableUndo) {
  if (!TextEditorPtr.IsValid() || CurrentFindText.IsEmpty()) {
    return false;
  }

  TSharedPtr<STextAssetEditableText> Editor = TextEditorPtr.Pin();
  FString FindStr = CurrentFindText.ToString();
  FString ReplaceStr = CurrentReplaceText.ToString();
  FString FullStr = EditingAsset->Content.ToString();

  LastFindText = FindStr;
  LastReplaceText = ReplaceStr;

  ESearchCase::Type CaseMode = bCaseSensitive ? ESearchCase::CaseSensitive : ESearchCase::IgnoreCase;

  int32 StartFrom = Editor->LocationToOffset(Editor->GetCursorLocation());
  int32 FoundAt = FullStr.Find(FindStr, CaseMode, ESearchDir::FromStart, StartFrom);

  if (FoundAt == INDEX_NONE) {
    FoundAt = FullStr.Find(FindStr, CaseMode, ESearchDir::FromStart, 0);
  }

  if (FoundAt != INDEX_NONE) {
    FString NewText = FullStr.Left(FoundAt) + ReplaceStr + FullStr.Mid(FoundAt + FindStr.Len());

    if (bEnableUndo) {
      const FScopedTransaction Transaction(LOCTEXT("ReplaceText", "Replace Text"));
      EditingAsset->Modify();
      EditingAsset->Content = FText::FromString(NewText);
    } else {
      EditingAsset->Content = FText::FromString(NewText);
    }

    Editor->SetText(FText::FromString(NewText));

    int32 NewCursorPos = FoundAt + ReplaceStr.Len();
    FTextLocation NewCursor = Editor->OffsetToLocation(NewCursorPos);
    Editor->SetCursorLocation(NewCursor);
    Editor->ScrollTo(NewCursorPos);

    CloseFindReplaceDialog();
    FSlateApplication::Get().SetKeyboardFocus(TextEditorPtr.Pin(), EFocusCause::SetDirectly);
    return true;
  }

  FNotificationInfo Info(FText::Format(LOCTEXT("NotFoundFmt", "'{0}' not found"), CurrentFindText));
  Info.ExpireDuration = 3.0f;
  FSlateNotificationManager::Get().AddNotification(Info);

  return false;
}

void FTextAssetUEEditorToolkit::PerformReplaceAll() {
  if (!TextEditorPtr.IsValid() || CurrentFindText.IsEmpty()) {
    return;
  }

  const FScopedTransaction Transaction(LOCTEXT("ReplaceAllText", "Replace All"));
  EditingAsset->Modify();

  int32 Count = 0;
  while (PerformReplace(false)) {
    Count++;
  }

  if (Count > 0) {
    FNotificationInfo Info(FText::Format(LOCTEXT("ReplacedCount", "Replaced {0} occurrence(s)"), FText::AsNumber(Count)));
    Info.ExpireDuration = 3.0f;
    FSlateNotificationManager::Get().AddNotification(Info);
  }
  else {
    FNotificationInfo Info(LOCTEXT("NoReplacements", "No occurrences found"));
    Info.ExpireDuration = 3.0f;
    FSlateNotificationManager::Get().AddNotification(Info);
  }
}


//UToolMenus::RegisterStartupCallback(
//  FSimpleMulticastDelegate::FDelegate::CreateRaw(
//    this,
//    &FTextAssetUEEditorModule::RegisterToolbar
//  )
//);
//

//FTextAssetUEEditorToolkit* FTextAssetUEEditorModule::FindActiveTextAssetToolkit() {
//  if (!GEditor) return nullptr;
//
//  UAssetEditorSubsystem* Subsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
//  if (!Subsystem) return nullptr;
//
//  for (UObject* Asset : Subsystem->GetAllEditedAssets()) {
//    IAssetEditorInstance* EditorInstance = Subsystem->FindEditorForAsset(Asset, false);
//    if (!EditorInstance) continue;
//    FTextAssetUEEditorToolkit* Toolkit = static_cast<FTextAssetUEEditorToolkit*>(EditorInstance);
//    if (Toolkit->GetTabManager()->GetOwnerTab()->IsForeground()) {
//      return Toolkit;
//    }
//  }
//
//  return nullptr;
//}

void FTextAssetUEEditorToolkit::ExtendToolbar() {
  TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender());

  ToolbarExtender->AddToolBarExtension(
    "Asset",
    EExtensionHook::After,
    GetToolkitCommands(),
    FToolBarExtensionDelegate::CreateRaw(
      this,
      &FTextAssetUEEditorToolkit::FillToolbar
    )
  );

  AddToolbarExtender(ToolbarExtender);
}

void FTextAssetUEEditorToolkit::FillToolbar(FToolBarBuilder& ToolbarBuilder) {
  ToolbarBuilder.BeginSection("TextAsset");

  ToolbarBuilder.AddToolBarButton(
    FUIAction(
      FExecuteAction::CreateRaw(
        this,
        &FTextAssetUEEditorToolkit::OnFindReplaceClicked
      )
    ),
    NAME_None,
    LOCTEXT("FindReplaceBtn", "Find / Replace"),
    LOCTEXT("FindReplaceTooltip", "Open find and replace dialog"),
    FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Search")
  );

  ToolbarBuilder.AddSeparator();

  ToolbarBuilder.AddToolBarButton(
    FUIAction(
      FExecuteAction::CreateRaw(
        this,
        &FTextAssetUEEditorToolkit::OnAppearanceClicked
      )
    ),
    NAME_None,
    LOCTEXT("AppearanceBtn", "Appearance"),
    LOCTEXT("AppearanceTooltip", "Open the text appearance dialog"),
    FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Settings")
  );

  ToolbarBuilder.AddSeparator();

  ToolbarBuilder.AddWidget(
    SNew(SBox)
    .HAlign(HAlign_Right)
    .VAlign(VAlign_Center)
    [
      SAssignNew(CursorPositionLabel, STextBlock)
        .Text(LOCTEXT("CursorPos", "Ln 1, Col 1"))
    ]
  );

  ToolbarBuilder.EndSection();
}

void FTextAssetUEEditorToolkit::OnFindReplaceClicked() {
  FTextAssetUEEditorToolkit::OpenFindReplaceDialog();
}

void FTextAssetUEEditorToolkit::OnAppearanceClicked() {
  FTextAssetUEEditorToolkit::OpenAppearanceDialog();
}

bool FTextAssetUEEditorToolkit::UpdateCursorPositionText(float DeltaTime) {
  if (!CursorPositionLabel.IsValid()) { return true; }
  if (TSharedPtr<STextAssetEditableText> Editor = TextEditorPtr.Pin()) {
    CursorPositionLabel->SetText(Editor->GetCursorPositionText());
  }

  return true;
}

#undef LOCTEXT_NAMESPACE
#include "TextAssetUEEditorFactory.h"
#include "TextAssetUE.h"

UTextAssetUEEditorFactory::UTextAssetUEEditorFactory() {
  SupportedClass = UTextAssetUE::StaticClass();
  bCreateNew = true;
  bEditAfterNew = true;
  bEditorImport = true;
  bText = true;
  Formats.Add(TEXT("txt;Text Asset UE File"));
  ImportPriority = 100;
}

UObject* UTextAssetUEEditorFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) {
  //return NewObject<UTextAssetUE>(InParent, InClass, InName, Flags);
  UTextAssetUE* NewAsset = NewObject<UTextAssetUE>(InParent, InClass, InName, Flags);

  if (UFactory::CurrentFilename.Len() > 0) {
    FString FilePath = UFactory::CurrentFilename;
    FString newContent;
    if (FFileHelper::LoadFileToString(newContent, *FilePath)) {
      NewAsset->Content = FText::FromString(newContent);
      UE_LOG(LogTemp, Log, TEXT("Loaded %d chars into Content"), newContent.Len());
    } else {
      UE_LOG(LogTemp, Warning, TEXT("Failed to load file content in fallback"));
    }
  } else {
    UE_LOG(LogTemp, Warning, TEXT("FactoryCreateNew: no filename available"));
  }
  if (!NewAsset->AssetImportData) {
    NewAsset->AssetImportData = NewObject<UAssetImportData>(NewAsset, NAME_None, RF_NoFlags);
  }
  NewAsset->AssetImportData->Update(CurrentFilename);
  NewAsset->Modify();
  return NewAsset;
}

FString UTextAssetUEEditorFactory::GetDefaultNewAssetName() const {
  return TEXT("NewTextAsset");
}

UClass* UTextAssetUEEditorFactory::GetSupportedClass() const {
  return UTextAssetUE::StaticClass();
}

bool UTextAssetUEEditorFactory::DoesSupportClass(UClass* Class) {
  return Class == UTextAssetUE::StaticClass();
}

bool UTextAssetUEEditorFactory::FactoryCanImport(const FString& Filename) {
  return FPaths::GetExtension(Filename, false).Equals(TEXT("txt"), ESearchCase::IgnoreCase);
}

bool UTextAssetUEEditorFactory::CanReimport(UObject* Obj, TArray<FString>& OutFilenames) {
  UTextAssetUE* TextAsset = Cast<UTextAssetUE>(Obj);
  if (TextAsset && TextAsset->AssetImportData) {
    FString SourceFile = TextAsset->AssetImportData->GetFirstFilename();
    if (!SourceFile.IsEmpty()) {
      OutFilenames.Add(SourceFile);
      return true;
    }
  }
  return false;
}

void UTextAssetUEEditorFactory::SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths) {
  UTextAssetUE* TextAsset = Cast<UTextAssetUE>(Obj);
  if (TextAsset && TextAsset->AssetImportData && NewReimportPaths.Num() > 0) {
    TextAsset->AssetImportData->Update(NewReimportPaths[0]);
  }
}

EReimportResult::Type UTextAssetUEEditorFactory::Reimport(UObject* Obj) {
  UTextAssetUE* TextAsset = Cast<UTextAssetUE>(Obj);
  if (!TextAsset || !TextAsset->AssetImportData) {
    return EReimportResult::Failed;
  }

  FString Filename = TextAsset->AssetImportData->GetFirstFilename();
  if (Filename.IsEmpty()) {
    return EReimportResult::Failed;
  }

  FString NewContent;
  if (!FFileHelper::LoadFileToString(NewContent, *Filename)) {
    return EReimportResult::Failed;
  }

  TextAsset->Modify();
  TextAsset->Content = FText::FromString(NewContent);
  TextAsset->PostEditChange();
  TextAsset->MarkPackageDirty();

  TextAsset->AssetImportData->Update(Filename);

  return EReimportResult::Succeeded;
}

int32 UTextAssetUEEditorFactory::GetPriority() const {
  return 0;
}
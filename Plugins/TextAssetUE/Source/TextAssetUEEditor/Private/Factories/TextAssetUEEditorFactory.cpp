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
    if (FFileHelper::LoadFileToString(NewAsset->Content, *FilePath)) {
      UE_LOG(LogTemp, Log, TEXT("Loaded %d chars into Content"), NewAsset->Content.Len());
    } else {
      UE_LOG(LogTemp, Warning, TEXT("Failed to load file content in fallback"));
    }
  } else {
    UE_LOG(LogTemp, Warning, TEXT("FactoryCreateNew: no filename available"));
  }

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

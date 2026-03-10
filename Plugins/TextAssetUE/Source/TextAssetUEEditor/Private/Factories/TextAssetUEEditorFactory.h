#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "EditorReimportHandler.h"
#include "TextAssetUEEditorFactory.generated.h"

UCLASS()
class TEXTASSETUEEDITOR_API UTextAssetUEEditorFactory : public UFactory, public FReimportHandler {
  GENERATED_BODY()

public:
  UTextAssetUEEditorFactory();

  virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
  virtual UClass* GetSupportedClass() const;
  virtual bool DoesSupportClass(UClass* Class) override;
  virtual bool FactoryCanImport(const FString& Filename);
  virtual bool ShouldShowInNewMenu() const override { return true; }
  virtual FString GetDefaultNewAssetName() const override;

  virtual bool CanReimport(UObject* Obj, TArray<FString>& OutFilenames) override;
  virtual void SetReimportPaths(UObject* Obj, const TArray<FString>& NewReimportPaths) override;
  virtual EReimportResult::Type Reimport(UObject* Obj) override;
  virtual int32 GetPriority() const override;
};

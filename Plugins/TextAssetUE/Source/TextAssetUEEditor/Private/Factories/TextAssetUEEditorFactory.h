#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "TextAssetUEEditorFactory.generated.h"

UCLASS()
class TEXTASSETUEEDITOR_API UTextAssetUEEditorFactory : public UFactory {
  GENERATED_BODY()

public:
  UTextAssetUEEditorFactory();

  // UFactory interface
  virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
  virtual UClass* GetSupportedClass() const;
  virtual bool DoesSupportClass(UClass* Class) override;
  virtual bool FactoryCanImport(const FString& Filename);
  virtual bool ShouldShowInNewMenu() const override { return true; }
  virtual FString GetDefaultNewAssetName() const override;
};

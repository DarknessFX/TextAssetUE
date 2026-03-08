#pragma once

#include "AssetTypeActions_Base.h"
#include "TextAssetUE.h"

class FAssetTypeActions_TextAssetUE : public FAssetTypeActions_Base {
public:
  FAssetTypeActions_TextAssetUE() {}

  virtual FText GetName() const override;
  virtual FColor GetTypeColor() const override;
  virtual uint32 GetCategories() override;
  virtual UClass* GetSupportedClass() const override;
  virtual void GetActions(const TArray<UObject*>& InObjects, FMenuBuilder& MenuBuilder) override;
  virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
  virtual const FSlateBrush* GetThumbnailBrush(const FAssetData& InAssetData, const FName InClassName) const override;
};
#pragma once

#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"

class FTextAssetUEEditorStyle : public FSlateStyleSet {
public:
  FTextAssetUEEditorStyle() : FSlateStyleSet("TextAssetUEEditorStyle") {
    const FString PluginBaseDir = IPluginManager::Get().FindPlugin(TEXT("TextAssetUE"))->GetBaseDir();
    SetContentRoot(FPaths::Combine(PluginBaseDir, TEXT("Resources")));

    const FVector2D Icon128x128(128.0f, 128.0f);
    const FVector2D Icon64x64(64.0f, 64.0f);

    Set("TextAssetUE.Thumbnail.Large", new FSlateImageBrush(RootToContentDir(TEXT("Icon128.png")), Icon128x128));
    Set("TextAssetUE.Thumbnail.Small", new FSlateImageBrush(RootToContentDir(TEXT("Icon128.png")), Icon64x64));

    Set("ClassIcon.TextAssetUE", new FSlateImageBrush(RootToContentDir(TEXT("Icon128.png")), Icon64x64));
    Set("ClassThumbnail.TextAssetUE", new FSlateImageBrush(RootToContentDir(TEXT("Icon128.png")), Icon128x128));

    FSlateStyleRegistry::RegisterSlateStyle(*this);
  }

  ~FTextAssetUEEditorStyle() {
    FSlateStyleRegistry::UnRegisterSlateStyle(*this);
  }

  static FTextAssetUEEditorStyle& Get() {
    static FTextAssetUEEditorStyle Instance;
    return Instance;
  }
};
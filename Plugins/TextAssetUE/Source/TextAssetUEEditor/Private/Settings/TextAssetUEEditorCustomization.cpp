#include "TextAssetUEEditorCustomization.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "PropertyHandle.h"
#include "Settings/TextAssetUEEditorSettings.h"

TSharedRef<IDetailCustomization> FTextAssetUEEditorCustomization::MakeInstance() {
  return MakeShareable(new FTextAssetUEEditorCustomization());
}

void FTextAssetUEEditorCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) {
  DetailBuilder.EditCategory("Appearance");

  TSharedPtr<IPropertyHandle> FontHandle = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UTextAssetUEEditorSettings, FontAsset));
  if (FontHandle.IsValid()) {
    DetailBuilder.HideProperty(FontHandle.ToSharedRef());  // optional hide default
    DetailBuilder.EditCategory("Appearance")
      .AddCustomRow(FontHandle->GetPropertyDisplayName())
      .NameContent()
      [
        FontHandle->CreatePropertyNameWidget(NSLOCTEXT("TextAssetUE", "FontFamily", "Font Family"))
      ]
      .ValueContent()
      [
        FontHandle->CreatePropertyValueWidget()
      ];
  }
}
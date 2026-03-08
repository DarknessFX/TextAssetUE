// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TextAssetUEEditor : ModuleRules {
	public TextAssetUEEditor(ReadOnlyTargetRules Target) : base(Target) 	{

		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { });
		PrivateIncludePaths.AddRange(new string[] {	});
	
		PublicDependencyModuleNames.AddRange(new string[]	{
      "Core",
      "CoreUObject",
      "Engine",
      "TextAssetUE",
      "UnrealEd",
      "AssetTools",
      "Slate",
      "SlateCore",
      "EditorStyle",
      "PropertyEditor",
      "Settings",
    });
		
		PrivateDependencyModuleNames.AddRange(new string[]	{
			"Projects",
		});
		
		DynamicallyLoadedModuleNames.AddRange(new string[] { });
	}
}

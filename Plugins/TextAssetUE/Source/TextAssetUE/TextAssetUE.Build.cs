// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TextAssetUE : ModuleRules {
	public TextAssetUE(ReadOnlyTargetRules Target) : base(Target) {

		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(new string[] { });
		PrivateIncludePaths.AddRange(new string[] { "TextAssetUEEditor/Private" });
		
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
      "CoreUObject",
      "Engine",
      "SlateCore",
    });
		
		PrivateDependencyModuleNames.AddRange(new string[] {
		});
		
		DynamicallyLoadedModuleNames.AddRange(new string[] { });
	}
}

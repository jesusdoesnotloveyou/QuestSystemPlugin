using UnrealBuildTool;

public class QuestSystemEditor : ModuleRules
{
	public QuestSystemEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"QuestSystemEditor/Public",
			}
		);
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"QuestSystemEditor/Private",
			}
		);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"UnrealEd",
			}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Slate",
				"SlateCore",
				"QuestSystemRuntime",
				"EditorStyle",
				"GraphEditor",
				"ToolMenus",
				"ApplicationCore"
			}
		);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
			}
		);
	}
}
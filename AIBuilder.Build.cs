// AIBuilder.Build.cs - Build configuration file
using UnrealBuildTool;

public class AIBuilder : ModuleRules
{
    public AIBuilder(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] 
        {
            "Core",
            "CoreUObject",
            "Engine",
            "AIModule",
            "GameplayTasks",
            "NavigationSystem",
            "UMG",
            "Slate",
            "SlateCore"
        });

        PrivateDependencyModuleNames.AddRange(new string[] 
        {
            "BehaviorTreeEditor",
            "BlueprintGraph",
            "KismetCompiler",
            "PropertyEditor",
            "EditorStyle",
            "EditorWidgets",
            "GraphEditor",
            "Kismet",
            "KismetWidgets",
            "SequenceRecorder"
        });

        if (Target.bBuildEditor == true)
        {
            PrivateDependencyModuleNames.AddRange(new string[] 
            {
                "UnrealEd",
                "ToolMenus"
            });
        }
    }
}

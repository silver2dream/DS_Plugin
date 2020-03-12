// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class DS_SQLLibrary : ModuleRules
{
	public DS_SQLLibrary(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Add the import library
            PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "x64", "include"));
            PublicLibraryPaths.Add(Path.Combine(ModuleDirectory, "x64", "lib"));

			PublicAdditionalLibraries.Add("mysqlcppconn.lib");
            PublicAdditionalLibraries.Add("mysqlcppconn-static.lib");

            // Delay-load the DLL, so we can load it from the right place first
            PublicDelayLoadDLLs.Add("mysqlcppconn.dll");
		}
	}
}

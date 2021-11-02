// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ChromaClient : ModuleRules
{
	public ChromaClient(ReadOnlyTargetRules Target) : base(Target)
	{
		//PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PCHUsage = ModuleRules.PCHUsageMode.NoSharedPCHs;
        //MinFilesUsingPrecompiledHeaderOverride = 0;  // or other larger numbers
        //Target.bForceIncludePrecompiledHeader = true;
        //// If this is set it seems to want to look in the Engine/Source folder. Specifying a file relative to this also doesn't seem to be sufficient.
        //SharedPCHHeaderFile = "";

        bUseRTTI = true;

        PublicDefinitions.Add("_CRT_HAS_CXX17=0");
        PublicDefinitions.Add("ECMULT_WINDOW_SIZE=15");
        PublicDefinitions.Add("ECMULT_GEN_PREC_BITS=4");

        PublicIncludePaths.AddRange(
			new string[] {
                "C:/Program Files/OpenSSL-Win64/include" // TODO - parametrize
            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "HTTP"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

        PublicAdditionalLibraries.AddRange(new string[] {
                "C:/Program Files/OpenSSL-Win64/lib/VC/libssl64MDd.lib",
                "C:/Program Files/OpenSSL-Win64/lib/VC/libcrypto64MDd.lib"
        });
    }
}
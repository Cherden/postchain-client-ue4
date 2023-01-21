// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using Tools.DotNETCommon;

public class ChromaClient : ModuleRules
{
	public ChromaClient(ReadOnlyTargetRules Target) : base(Target)
	{
		//PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PCHUsage = ModuleRules.PCHUsageMode.NoSharedPCHs;

		bUseRTTI = true;

		PublicDefinitions.Add("_CRT_HAS_CXX17=0");
		PublicDefinitions.Add("ECMULT_WINDOW_SIZE=15");
		PublicDefinitions.Add("ECMULT_GEN_PREC_BITS=4");
		PublicDefinitions.Add("CURL_STATICLIB");
		PublicDefinitions.Add("CHROMIA_INSIDE_UNREAL_ENGINE");

		AddEngineThirdPartyPrivateStaticDependencies(Target, "libcurl");
		AddEngineThirdPartyPrivateStaticDependencies(Target, "OpenSSL");

		string ChromiaPureInclude = System.IO.Path.GetFullPath(System.IO.Path.Combine(ModuleDirectory)) + "/chroma-cpp-pure";
		Log.TraceWarning("ChromiaPureInclude path: {0}", ChromiaPureInclude);

		PublicIncludePaths.AddRange(
			new string[] {
				ChromiaPureInclude + "/src",
				ChromiaPureInclude + "/tests"
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
                //"HTTP"
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
        });
    }
}

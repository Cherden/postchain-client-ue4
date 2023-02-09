// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

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

		AddEngineThirdPartyPrivateStaticDependencies(Target, new string[]{
			"OpenSSL",
			"libcurl"
		});

		// On UE5, two additional dependencies are needed
		BuildVersion Version;
        if (BuildVersion.TryRead(BuildVersion.GetDefaultFileName(), out Version))
        {
            if (Version.MajorVersion == 5)
            {
                AddEngineThirdPartyPrivateStaticDependencies(Target, new string[]{
                    "nghttp2",
                    "zlib"
                });
            }
		}

		string ChromiaPureInclude = System.IO.Path.GetFullPath(System.IO.Path.Combine(ModuleDirectory)) + "/chroma-cpp-pure";

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
				"SlateCore"
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


//// Copyright Epic Games, Inc. All Rights Reserved.

//using UnrealBuildTool;

//public class ChromaClient : ModuleRules
//{
//	public ChromaClient(ReadOnlyTargetRules Target) : base(Target)
//	{
//		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

//		PublicIncludePaths.AddRange(
//			new string[] {
//				// ... add public include paths required here ...
//			}
//			);


//		PrivateIncludePaths.AddRange(
//			new string[] {
//				// ... add other private include paths required here ...
//			}
//			);


//		PublicDependencyModuleNames.AddRange(
//			new string[]
//			{
//				"Core",
//				// ... add other public dependencies that you statically link with here ...
//			}
//			);


//		PrivateDependencyModuleNames.AddRange(
//			new string[]
//			{
//				"CoreUObject",
//				"Engine",
//				"Slate",
//				"SlateCore",
//				// ... add private dependencies that you statically link with here ...	
//			}
//			);


//		DynamicallyLoadedModuleNames.AddRange(
//			new string[]
//			{
//				// ... add any modules that your module loads dynamically here ...
//			}
//			);
//	}
//}

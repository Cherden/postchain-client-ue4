// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using Tools.DotNETCommon;

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
        //bEnableUndefinedIdentifierWarnings = false;

        PublicDefinitions.Add("_CRT_HAS_CXX17=0");
        PublicDefinitions.Add("ECMULT_WINDOW_SIZE=15");
        PublicDefinitions.Add("ECMULT_GEN_PREC_BITS=4");
        PublicDefinitions.Add("CURL_STATICLIB");
        PublicDefinitions.Add("CHROMIA_INSIDE_UNREAL_ENGINE");
        //PublicDefinitions.Add("WIN32_LEAN_AND_MEAN");
        // PublicDefinitions.Add("NDEBUG=1");

        string OpenSSLRoot = System.Environment.GetEnvironmentVariable("OPENSSL_ROOT_DIR");

        //Log.TraceWarning("OpenSSLRoot: " + OpenSSLRoot);
        //Log.TraceWarning("ModuleDir: " + ModuleDirectory);
        //Log.TraceWarning(System.IO.Path.Combine(ModuleDirectory, "../chroma-cpp-pure/src"));

        PublicIncludePaths.AddRange(
            new string[] {
                System.IO.Path.Combine(OpenSSLRoot, "include"),
                //"C:/Projects/Chroma/curlpp-0.8.1/include",
                "C:/Projects/Chroma/curl-7.80.0/include",
                "C:/Projects/Chroma/Clean Commit/googletest-release-1.11.0/googletest/include",
                //"C:/Projects/Chroma/curlcpp/include"
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
                //"C:/Program Files/OpenSSL-Win64/lib/VC/libssl64MDd.lib",
                //"C:/Program Files/OpenSSL-Win64/lib/VC/libcrypto64MDd.lib"
                System.IO.Path.Combine(OpenSSLRoot, "lib/VC/libssl64MDd.lib"),
                System.IO.Path.Combine(OpenSSLRoot, "lib/VC/libcrypto64MDd.lib"),
            //"C:/Projects/Chroma/curlpp-0.8.1/build/Release/curlpp.lib",
            //"C:/Projects/Chroma/curlpp-0.8.1/build/Release/libcurlpp.lib",

                "C:/Projects/Chroma/curl-7.80.0/build/lib/x64/libcurl.lib",
                //"C:/Projects/Chroma/curl-7.80.0/build/lib/x64/libcurld.lib",

                "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.17763.0/um/x64/Crypt32.Lib",
                "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.17763.0/um/x64/WS2_32.Lib",
                "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.17763.0/um/x64/WinMM.Lib",
                "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.17763.0/um/x64/Wldap32.Lib"
        });
    }
}


# Installation guide


## Pre-requirements

1. Unreal Engine (I used 4.27 for this, but there should be no problem with older versions)
2. Visual Studio (I tested on both 2017 and 2019)
3. Node JS

## Build

1. Clone` https://github.com/Cherden/postchain-client-cpp` under `\Plugins\ChromaClient\Source\ChromaClient\` .
Make sure new cloned dir name is `chroma-cpp-pure`
The structure should look like this at this moment: 
![image](https://user-images.githubusercontent.com/85849035/140369944-0f6673a5-8ff0-4a06-b97d-a1ae08dc9517.png)

2. Right click on `ChromaClientTest.uproject->GenerateProjectFiles` . Wait for `ChromaClientTest.sln` to be generated
5. Open `ChromaClientTest.sln`
6. Make sure build configuration is `Development Editor`  & `Win64`	
7. Build & Run solution.


## Configure

Open `Content/Postchain/PostchainDemo.uasset (Widget)`
Navigate to Graph -> Variables list
Modify `Blockchain_BRID` & `Blockchain_URL` with correct values

Open Content/SSO/SSODemoWidget.uasset (Widget)
Navigate to Graph -> Variables list
Modify `Blockchain_BRID` & `Blockchain_URL` with correct values


## Prepare SSO Auth Server

This will launch a small js server wich will intercept the rawTx code from SSO auth success, and transfers it to UE4 plugin, via some json file. 
```
cd SSOAuthServer/
node index.js
``` 


## Test
 
### For SSO Demo:
Open `Content/SSODemoLevel`

### For Postchain Demo:
Open `Content/PostchainDemoLevel`

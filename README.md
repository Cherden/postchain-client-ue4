
# Installation guide


## Pre-requirements

1. Unreal Engine (I used 4.27 for this, but there should be no problem with older versions)
2. openssl
3. Visual Studio

  

## Setup

1. Clone` https://github.com/Cherden/postchain-client-cpp` under `\Plugins\ChromaClient\Source\ChromaClient\` .
Make sure dir name remains `postchain-client-cpp`

2. Create new environment variable (if not already existing):

```sh
OPENSSL_ROOT_DIR="/path/to/openssl"
```
Eg: `C:/Program Files/OpenSSL-Win64/`

3. Right click on `ChromaClientTest.uproject->GenerateProjectFiles` . Wait for `ChromaClientTest.sln` to be generated

5. Open `ChromaClientTest.sln`
6. Make sure build configuration is `Development Editor`  & `Win64`	
7. Build & Run solution.

### Development Note: 
At this moment, blockchain URL, RID and username to be inserted in DB, are hardcoded inside `postchain-client-ue4\Source\ChromaClientTest\ChromaClientTestGameModeBase.cpp`
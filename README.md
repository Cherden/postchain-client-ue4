
# Installation guide


## Pre-requirements

1. Unreal Engine 4 or 5
2. Visual Studio (I tested on both 2017 and 2019)
3. Node JS
4. Docker(Optional)

## Build Unreal Engine plugin

##  !!! VERY IMPORTANT: IF YOU WANT TO MODIFY UASSETS, PLEASE DO THIS IN UE4 VERSION OR ON A SEPARATED BRANCH, BECAUSE YOU WILL NOT BE ABLE TO USE UASSETS MODIFIED IN UE4 ON UE5. UNFORTUNATELY UASSETS ARE NOT BACK COMPATIBLE. 

1. Clone` https://github.com/Cherden/postchain-client-cpp` under `\Plugins\ChromaClient\Source\ChromaClient\` .
Make sure new cloned dir name is `chroma-cpp-pure`
The structure should look like this at this moment: 
![image](https://user-images.githubusercontent.com/85849035/140369944-0f6673a5-8ff0-4a06-b97d-a1ae08dc9517.png)

2. Right click on `ChromaClientTest.uproject->GenerateProjectFiles` . Wait for `ChromaClientTest.sln` to be generated. 
   Note: If you want to change Engine version, then:
    - `git clean -fdx`
    - Right click on `ChromaClientTest.uproject->SwitchUnrealEngineVersion`
3. Open `ChromaClientTest.sln`
4. Make sure build configuration is `Development Editor`  & `Win64`	
5. Build & Run solution.

## Configure blockchain
1. Generate a new admin keypair with optional keygen. If running locally the following test keys can be used:
    - `private key: 3132333435363738393031323334353637383930313233343536373839303131`
    - `public key: 0350fe40766bc0ce8d08b3f5b810e49a8352fdd458606bd5fafe5acdcdc8ff3f57` 

2. Open `Config/blockchain-config.sample.json` and update private key. Brid and url will be automatically set
 
3. Go into Rell/rell/config/local/run.xml and put the public key in
```
    <args module="common">
      <arg key="admin_pubkey">
        <bytea>PUBKEY HERE</bytea>
      </arg>
    </args>
```
4. Copy the file /Administration/.env.sample, name it .env.
5. Open the .env file you juts created in the Administration folder and paste the private key after "ADMIN_PRIV_KEY=". Save it
6. Run blockchain
- If you want to run blockchain with Docker: 
  - `Open terminal as Administrator`
  - `cd /Administration`  
  - `npm i`
  - `npm start`
- If you don't want to use Docker, then:
  - Open and run ./Rell project with Eclipse (https://docs.chromia.com/tools/eclipse/installation)
  - `Open terminal as Administrator`
  - `cd /Administration`
  - `npm i`
  - `node init_blockchain.js`
7. After this point restart `ChromaClientTest.sln` Visual studio instance, and UE4 project, for some environment variables to be updated.
8. Use a browser (not Mozilla Firefox!) to go to http://localhost:7740/brid/iid_0. Copy the RID shown.
9. Open https://dev.vault.chromia-development.com/ and create a Custom D-App. Name: `Chromia Unity`, Host: `http://localhost`, Port: `7740`, Website: `http://localhost`, and paste the RID into ChainID.


## Test PostchainDemo scene
1. Open `Content/PostchainDemoLevel`
2. Play

## Test SSODemo scene
1. Prepare node local auth script. This will launch a small js server wich will intercept the rawTx code from SSO auth success, and transfers it to UE4 plugin, via some json file. 
  - `cd SSOAuthServer/`
  - `node index.js`
2. Open `Content/SSODemoLevel`
3. Play

## Test LoginUserDemo scene
### In this scene you can create/remove players, and give assets to them.
1. Open `Content/LoginUserDemoLevel`
2. Play


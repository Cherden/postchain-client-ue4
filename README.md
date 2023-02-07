
# Installation guide


## Pre-requirements

1. Unreal Engine (Use 4.27 for `dev` branch and 5.1.1 for `ue5` branch)
2. Visual Studio (I tested on both 2017 and 2019)
3. Node JS
4. Docker(Optional)

## Build Unreal Engine plugin

1. Clone` https://github.com/Cherden/postchain-client-cpp` under `\Plugins\ChromaClient\Source\ChromaClient\` .
Make sure new cloned dir name is `chroma-cpp-pure`
The structure should look like this at this moment: 
![image](https://user-images.githubusercontent.com/85849035/140369944-0f6673a5-8ff0-4a06-b97d-a1ae08dc9517.png)

2. Right click on `ChromaClientTest.uproject->GenerateProjectFiles` . Wait for `ChromaClientTest.sln` to be generated
5. Open `ChromaClientTest.sln`
6. Make sure build configuration is `Development Editor`  & `Win64`	
7. Build & Run solution.

## Test PostchainDemo scene
1. Open `Content/Postchain/PostchainDemo.uasset (Widget)`
2. Navigate to Graph -> Variables list
3. Modify `Blockchain_BRID` & `Blockchain_URL` with correct values
4. Open `Content/PostchainDemoLevel`
5. Play

## Test SSODemo scene
1. Prepare node local auth script. This will launch a small js server wich will intercept the rawTx code from SSO auth success, and transfers it to UE4 plugin, via some json file. 
`cd SSOAuthServer/`
`node index.js`
2. Open `Content/SSO/SSODemoWidget.uasset (Widget)`
3. Navigate to Graph -> Variables list
4. Modify `Blockchain_BRID` & `Blockchain_URL` with correct values
5. Open `Content/SSODemoLevel`
6. Play

## Test LoginUserDemo scene
### In this scene you can create/remove players, and give assets to them.
1. Generate a new admin keypair with optional keygen. If running locally the following test keys can be used:
    `private key: 3132333435363738393031323334353637383930313233343536373839303131`
    `public key: 0350fe40766bc0ce8d08b3f5b810e49a8352fdd458606bd5fafe5acdcdc8ff3f57`  
2. Go into Rell/rell/config/local/run.xml and put the public key in
```
    <args module="common">
      <arg key="admin_pubkey">
        <bytea>PUBKEY HERE</bytea>
      </arg>
    </args>
```
3. Copy the file /Administration/.env.sample, name it .env.
4. Open the .env file you juts created in the Administration folder and paste the private key after "ADMIN_PRIV_KEY=". Save it.
5.
- If you want to run blockchain with Docker: 
  - `cd /Administration`  
  - `npm i`
  - `npm start`
- If you don't want to use Docker, then:
  - Open and run ./Rell project with Eclipse (https://docs.chromia.com/tools/eclipse/installation)
  - `cd /Administration`
  - `npm i`
  - `node init_blockchain.js`
6. Use a browser (not Mozilla Firefox!) to go to http://localhost:7740/brid/iid_0. Copy the RID shown.
7. Open https://dev.vault.chromia-development.com/ and create a Custom D-App. Name: `Chromia Unity`, Host: `http://localhost`, Port: `7740`, Website: `http://localhost`, and paste the RID into ChainID.
8. Open `Content/Postchain/PostchainDemo.uasset (Widget)`
9. Navigate to Graph -> Variables list
10. Modify `Blockchain_BRID` & `Blockchain_URL` with correct values
11. Open `Content/LoginUserDemoLevel`
12. Play


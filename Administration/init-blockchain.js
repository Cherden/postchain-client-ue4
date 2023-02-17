const {
  Postchain,
  KeyPair,
  User,
  SingleSignatureAuthDescriptor,
  FlagsType,
  op,
  nop,
} = require("ft3-lib");
const fs = require("fs");
const request = require("requestretry");
const path = require("path");
require("dotenv").config();
var exec = require('child_process').exec;

class RewardFT3 {
  constructor(token, amount) {
    this.token = token;
    this.amount = amount;
  }

  toGTV() {
    return [0, [this.token, this.amount]];
  }
}

class RewardOriginal {
  constructor(name) {
    this.name = name;
  }

  toGTV() {
    return [1, [this.name]];
  }
}

const url = process.env.NODE_URL;
const adminPrivKey = process.env.ADMIN_PRIV_KEY;

// Add all your assets here
const assetCategories = require("./GameConfigs/migrated-asset-list.json");
const trashItems = require("./GameConfigs/trash_items.json");
const nftExampleCategories = require("./GameConfigs/NFTs/nftexample.json");

async function getBlockchainRid() {
  const brid = await request({
    url: `${url}/brid/iid_0`,
    fullResponse: false,
    maxAttempts: 20,
    retryDelay: 5000,
    retryStrategy,
  });

  if (brid.match(/^[a-fA-F0-9]{64}$/i) === null) {
    throw new Error(
      `Did the blockchain start properly? Received invalid BRID: ${brid}`
    );
  }

  return brid;
}

function retryStrategy(err, response, body, options) {
  if (response && response.attempts >= options.maxAttempts) {
    console.log(`Failed with status code: ${response.statusCode}`);
    if (body) console.log(body);
    if (err) console.error(err);
  }

  return !!err || response.statusCode !== 200 || body.includes("error");
}

function createReward(rewardJson) {
  const { type, reward } = rewardJson;
  if (type === "ft3") {
    return new RewardFT3(reward.name, reward.amount);
  } else if (type === "original") {
    return new RewardOriginal(findAssetIdByName(reward.name));
  } else {
    throw new Error("Unknown reward type");
  }
}

function findAssetIdByName(name) {
  for (let category of Object.values(assetCategories)) {
    for (let asset of category) {
      if (asset.name === name) return asset.itemId;
    }
  }

  console.log(`No such asset: ${name}`);

  return "";
}

async function executeTimedTransaction(transaction, name) {
  const timestamp = Date.now();
  await transaction.post();
  const duration = Date.now() - timestamp;

  if (duration >= 9000) {
    console.log(
      `\n🚨 Threshold reached! Transaction '${name}' executed in ${duration}ms 🚨`
    );
  } else {
    console.log(`\n⏰ Transaction '${name}' executed in ${duration}ms`);
  }
}

function setEnv(name, value, cb) {
  if (!name) throw new Error('name required.');
  if (typeof value === 'undefined') throw new Error('value required.');

  var cmd;
  if (value.indexOf('+=') === 0) value = process.env[name] + value.substring(2);
  value = value.indexOf(' ') > -1 ? `"${value}"` : value;

  switch (process.platform) {
    case 'linux': cmd = `export ${name}=${value}`; break;
    case 'win32': cmd = `setx ${name} ${value}`; break;
    default: throw new Error('unsupported platform.'); break;
  }

  exec(cmd, cb);
}

(async function () {
  console.log("⏳ Getting blockchainID, please wait...");
  const brid = await getBlockchainRid();

  // update BRID in Unity project
  const configDir = path.join(
    __dirname,
    "..",
    "Config"
  );
  const sampleFile = require(path.join(
    configDir,
    "blockchain-config.sample.json"
  ));

  function setEnvCB(error, stdout, stderr) {
    console.log(`setEnvCB() error: ${error}`);
    console.log(`setEnvCB() stdout: ${stdout}`);
    console.log(`setEnvCB() stderr: ${stderr}`);
  }

  // Write values to env
  setEnv('CHROMA_BLOCKCHAIN_URL', sampleFile.blockchainUrl, setEnvCB);
  setEnv('CHROMA_BRID', brid, setEnvCB);
  setEnv('CHROMA_PRIV_KEY', sampleFile.privKey, setEnvCB);

  // Old version, write json to config file
  // sampleFile.brid = brid;
  // const configFile = path.join(configDir, "blockchain-config.json");
  // fs.writeFileSync(configFile, JSON.stringify(sampleFile, null, 2), (error) => {
  //   if (error) return console.log(error);
  //   console.log(`Saved BRID ${brid} in ${configFile}`);
  // });

  // initialize blockchain
  const blockchain = await new Postchain(url).blockchain(brid);

  const adminKeyPair = new KeyPair(adminPrivKey);

  const adminUser = new User(
    adminKeyPair,
    new SingleSignatureAuthDescriptor(adminKeyPair.pubKey, [
      FlagsType.Account,
      FlagsType.Transfer,
    ])
  );

  console.log("\n🤑 Registering game assets...\n");

  const gameAssetsTransactionBuilder = blockchain
    .transactionBuilder()
    .add(op("admin.initialize"))
    .add(op("assets.initialize"));

  // eslint-disable-next-line array-callback-return
  // Object.entries(assetCategories).map(([interfaceName, assets]) => {
  //   if (interfaceName === "FT3") {
  //     for (let { name, hidden_in_inventory } of assets) {
  //       gameAssetsTransactionBuilder.add(
  //         op("assets.register_ft3_asset", name, !!hidden_in_inventory)
  //       );
  //     }
  //   } else {
  //     for (const { name, file } of assets) {
  //       gameAssetsTransactionBuilder.add(
  //         op(
  //           "assets.register_original_asset",
  //           name,
  //           interfaceName,
  //           file.url,
  //           Buffer.from(file.brid, "hex"),
  //           Buffer.from(file.hash, "hex")
  //         )
  //       );
  //     }
  //   }
  // });

  Object.entries(assetCategories).map(([interfaceName, assets]) => {
    if (interfaceName === "FT3") {
      for (let { name, hidden_in_inventory } of assets) {
        gameAssetsTransactionBuilder.add(
          op("assets.register_ft3_asset", name, !!hidden_in_inventory)
        );
      }
    } else if (interfaceName == "com.myneighboralice.ICostume") {
      for (const { name, hub_location, hub_brid, hash } of assets) {
        gameAssetsTransactionBuilder.add(
          op(
            "assets.register_original_asset",
            name,
            interfaceName,
            hub_location,
            Buffer.from(hub_brid, "hex"),
            Buffer.from(hash, "hex")
          )
        );
      }
    } else {
      for (const { name, file } of assets) {
        gameAssetsTransactionBuilder.add(
          op(
            "assets.register_original_asset",
            name,
            interfaceName,
            file.url,
            Buffer.from(file.brid, "hex"),
            Buffer.from(file.hash, "hex")
          )
        );
      }
    }
  });

  await executeTimedTransaction(
    gameAssetsTransactionBuilder.add(nop()).buildAndSign(adminUser),
    "Register game assets"
  );

  const questsTransactionBuilder = blockchain.transactionBuilder();

  
  console.log("\n🚮 Importing trash items...\n");
  const trashImportBuilder = blockchain.transactionBuilder();
  for (const trash of trashItems) {
    trashImportBuilder.add(op("assets.add_trash", trash));
  }
  await executeTimedTransaction(
    trashImportBuilder.add(nop()).buildAndSign(adminUser),
    "Import trash assets"
  );

  // TODO: Temporarily disabled until out the exact issue Unity has with the assets
  // await executeTimedTransaction(
  //   farmingTransactionBuilder.add(nop()).buildAndSign(adminUser),
  //   "Register farming data"
  // );

  console.log("\nChain initialized! 🎉");
})();

require("dotenv").config();

const fs = require("fs");
const assets = require("./GameConfigs/asset-list.json");
const { Filehub, FsFile } = require("@snieking/fs-client");
const {
  FlagsType,
  KeyPair,
  SingleSignatureAuthDescriptor,
  User,
  Postchain,
} = require("ft3-lib");

async function main() {
  const filehubUrl =
    "https://gl90tpkjck.execute-api.eu-central-1.amazonaws.com/dev/";
  const filehubBrid =
    "9C6BFD0B96803A131C1EED2ED773097E2DB5B4103B7CBD5CD23701D1F1D43C02";
  const user = await createFt3User(
    filehubUrl,
    filehubBrid,
    process.env.ADMIN_PRIV_KEY
  );
  const filehub = new Filehub(filehubUrl, filehubBrid);

  const updatedAssets = {};
  for (const interface of Object.keys(assets)) {
    if (interface === "FT3") {
      updatedAssets.FT3 = assets["FT3"];
      continue;
    }

    const updatedInterface = [];
    for (const asset of assets[interface]) {
      const updatedAsset = { name: asset.itemId };
      let hash = "";

      if (asset.image) {
        const file = FsFile.fromData(Buffer.from(asset.image, "base64"));
        if (!(await filehub.hasFile(file.hash))) {
          try {
            await filehub.storeFile(user, file);
          } catch (e) {
            console.log(e);
          }
        }
        hash = file.hash.toString("hex");
      }

      updatedAsset.file = {
        url: filehubUrl,
        brid: filehubBrid,
        hash,
      };

      updatedInterface.push(updatedAsset);
    }

    updatedAssets[interface] = updatedInterface;
  }

  fs.writeFileSync(
    "./GameConfigs/migrated-asset-list.json",
    JSON.stringify(updatedAssets, null, 2)
  );
}

async function createFt3User(filehubUrl, filehubBrid, privKey) {
  const blockchain = await new Postchain(filehubUrl).blockchain(filehubBrid);

  const walletKeyPair = new KeyPair(
    privKey ? privKey : makeKeyPair().privKey.toString("hex")
  );
  const walletAuthDescriptor = new SingleSignatureAuthDescriptor(
    walletKeyPair.pubKey,
    [FlagsType.Account, FlagsType.Transfer]
  );

  const walletUser = new User(walletKeyPair, walletAuthDescriptor);

  try {
    await blockchain.registerAccount(walletAuthDescriptor, walletUser);
  } catch (error) {
    console.log("User already registered");
  }

  return new Promise((resolve) => resolve(walletUser));
}

main();

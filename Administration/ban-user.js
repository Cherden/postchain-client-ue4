const {
  Postchain,
  op,
  nop,
  User,
  FlagsType,
  SingleSignatureAuthDescriptor,
} = require("ft3-lib");
const bip39 = require("bip39");
const bip32 = require("bip32");

const gameMasterVaultSeedPhrase = ""; // Make sure to not commit your seed phrase, it's secret :-)

const nodeUrl = "";
const brid = "";

const usernameToBan = "";
const daysToBan = 1;

const getEpochTimeDaysFromNow = (days) => {
  return new Date(Date.now() + 86400000 * days).getTime();
};

async function mnemonicToUser(mnemonic) {
  const masterKeyNode = await masterKeyNodeFromMnemonic(mnemonic);
  return userFrom(masterKeyNode);
}

async function masterKeyNodeFromMnemonic(mnemonic) {
  const seed = await bip39.mnemonicToSeed(mnemonic);
  return bip32.fromSeed(seed);
}

userFrom = (masterKeyNode) => {
  const keyPair = {
    privKey: masterKeyNode.privateKey,
    pubKey: masterKeyNode.publicKey,
  };

  return new User(
    keyPair,
    new SingleSignatureAuthDescriptor(keyPair.pubKey, [
      FlagsType.Account,
      FlagsType.Transfer,
    ])
  );
};

async function main() {
  const gameMaster = await mnemonicToUser(gameMasterVaultSeedPhrase);
  const id = gameMaster.authDescriptor.id;
  const banDuration = getEpochTimeDaysFromNow(daysToBan);

  const blockchain = await new Postchain(nodeUrl).blockchain(brid);

  await blockchain
    .transactionBuilder()
    .add(nop())
    .add(
      op(
        "game_master.assign_flag",
        "Banned",
        usernameToBan,
        banDuration,
        id,
        id
      )
    )
    .buildAndSign(gameMaster)
    .post();
}

main();

require("dotenv").config();
const {
  Postchain,
  KeyPair,
  User,
  SingleSignatureAuthDescriptor,
  FlagsType,
  op,
  nop,
} = require("ft3-lib");
const request = require("requestretry");

// Configure these values to your liking before running the script

// Users can be promoted either via username or accountId
// accountId makes sense when the user isn't registered yet
const username = "";
const accountId = ""; // Can be taken from Vault website
const promote = true;
const validTo = monthsInTheFuture(1);
// --------------------------

const url = process.env.NODE_URL;
const adminPrivKey = process.env.ADMIN_PRIV_KEY;

async function getBlockchainRid() {
  return request({
    url: `${url}/brid/iid_0`,
    fullResponse: false,
    maxAttempts: 20,
    retryDelay: 1000,
    retryStrategy,
  });
}

function retryStrategy(err, response, body, options) {
  return !!err || response.statusCode !== 200;
}

function monthsInTheFuture(months) {
  return new Date(
    new Date().getTime() + months * 30 * 24 * 60 * 60 * 1000
  ).getTime();
}

(async function () {
  const brid = await getBlockchainRid();

  if (!brid) {
    console.error(
      "Failed to get blockchain RID, please check if the blockchain ran successfully"
    );
    process.exit(1);
  }

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

  const trxBuilder = blockchain.transactionBuilder().add(nop());

  if (promote) {
    if (username) {
      console.log("=============================================");
      console.log(`Promoting user ${username} to game master until ${validTo}`);
      console.log("=============================================");
      trxBuilder.add(
        op(
          "game_master.assign_flag",
          "Game Master",
          username,
          validTo,
          adminUser.authDescriptor.id,
          adminUser.authDescriptor.id
        )
      );
    } else if (accountId) {
      console.log("=============================================");
      console.log(
        `Promoting user ${accountId} to game master until ${validTo}`
      );
      console.log("=============================================");
      trxBuilder.add(
        op(
          "game_master.assign_flag_by_account_id",
          "Game Master",
          Buffer.from(accountId, "hex"),
          validTo,
          adminUser.authDescriptor.id,
          adminUser.authDescriptor.id
        )
      );
    }
  } else {
    if (username) {
      console.log("=============================================");
      console.log(`Demoting user ${username}`);
      console.log("=============================================");
      trxBuilder.add(
        op(
          "game_master.revoke_flag",
          "Game Master",
          username,
          adminUser.authDescriptor.id,
          adminUser.authDescriptor.id
        )
      );
    } else if (accountId) {
      console.log("=============================================");
      console.log(`Demoting user ${accountId}`);
      console.log("=============================================");
      trxBuilder.add(
        op(
          "game_master.revoke_flag_by_account_id",
          "Game Master",
          Buffer.from(accountId, "hex"),
          adminUser.authDescriptor.id,
          adminUser.authDescriptor.id
        )
      );
    }
  }

  trxBuilder.buildAndSign(adminUser).post();
})();

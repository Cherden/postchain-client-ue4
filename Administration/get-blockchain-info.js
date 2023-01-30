require("dotenv").config();
const request = require("requestretry");

const { Postchain } = require("ft3-lib");

const url = process.env.NODE_URL;

const log = console.log;
let logEnabled = false;

console.log = function (message) {
  if (logEnabled) {
    log(message);
  }
};

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

async function main() {
  const brid = await getBlockchainRid();
  const blockchain = await new Postchain(url).blockchain(brid);

  const data = await blockchain.query("game_info.get_all", {});
  logEnabled = true;
  console.log(JSON.stringify(data));
}

main();

const fs = require("fs");

const args = process.argv.slice(2);

const fileName = args[0];
const blockchain = args[1];
const brid = args[2];

const bridRegexp = /[A-Fa-f0-9]{64}/i;

console.log(`File Name: ${fileName}`);
console.log(`Blockchain: ${blockchain}`);
console.log(`Brid: ${brid}`);

if (!fileName || !blockchain || !brid) {
  console.log("Skipping updating due to missing arguments");
  process.exit(1);
} else if (!bridRegexp.test(brid)) {
  console.log("Skipping updating due to invalid brid");
  process.exit(1);
}

const content = fs.readFileSync(fileName, "utf8");
const lines = content.split("\n");

for (let i = 0; i < lines.length; i++) {
  if (lines[i].includes(blockchain)) {
    var lineNr = i - 1;
    var ridLine = lines[lineNr];
    lines[lineNr] = ridLine.replace(bridRegexp, brid);
  }
}

fs.writeFileSync(fileName, lines.join("\n"));

const { spawn, spawnSync } = require("child_process");
const path = require("path");

const rellDirectory = path.join(__dirname, "..", "Rell", "rell");
console.log("Executing tests for", rellDirectory);

function verifyDocker() {
  const { status } = spawnSync(`docker ps`, { shell: true });

  if (status !== 0) {
    console.error("Docker is not running");
    process.exit(1);
  }
}

async function test() {
  const task = await spawn(
    `docker run --rm -v ${rellDirectory}:/usr/src/rell -e RUN_XML=config/test/run.xml snieking/postchain test`,
    { shell: true }
  );

  let dbInitialized = false;
  task.stdout.on("data", (data) => {
    const message = data.toString();
    if (message.includes("PostgreSQL init process complete")) {
      dbInitialized = true;
    } else if (dbInitialized && isLogOfInterest(data)) {
      console.log(message.trim());
    }
  });

  let systemInitialized = false;
  task.stderr.on("data", (data) => {
    const message = data.toString();
    if (
      !systemInitialized &&
      message.includes("database system is ready to accept connections")
    ) {
      systemInitialized = true;
    } else if (systemInitialized) {
      console.error(message.trim());
    }
  });

  task.on("close", (code) => {
    process.exit(code);
  });

  task.on("error", (error) => {
    console.error(error);
    process.exit(1);
  });
}

function isLogOfInterest(data) {
  return !data.includes("SqlInit") && !data.includes("SQLDatabaseAccess");
}

async function main() {
  verifyDocker();
  await test();
}

main();

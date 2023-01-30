#!/bin/bash
set -e

LOCAL_POSTCHAIN_BIN_DIR=~/.rell/bin/0.10.7/postchain-node

BLOCKCHAIN_DIR="$(dirname ${BASH_SOURCE[0]})/../Rell"

if [ "$#" -ne 4 ]; then
    echo "ERROR: Missing arguments"
    echo "Usage: $(basename $BASH_SOURCE) <environment> <user> <node> <nodeUrl>"
    exit 1
else 
    ENVIRONMENT="$1"
    REMOTE_USER="$2"
    POSTCHAIN_NODE="$3"
    NODE_URL="$4"
fi

REMOTE_CONTRACT_DIR=/home/${REMOTE_USER}/myneighbor-alice
REMOTE_POSTCHAIN_BIN_DIR=/home/${REMOTE_USER}/postchain-10.7

VAULT_LOCAL_DIR="/tmp/wallet";

case "$ENVIRONMENT" in
    environment/dev|environment/test|environment/stage)
        VAULT_BRANCH="development"
        VAULT_CONFIG_FILE="src/config/config-dev.js";
        ;;
    environment/release)
        ;;
    *)
        echo "ERROR: Unknown environment: $ENVIRONMENT"
        exit 2
        ;;
esac

# compile contract
case `uname -s` in
    MINGW*)
        RELL_LIB="$(realpath --relative-to="$PWD" $(realpath $LOCAL_POSTCHAIN_BIN_DIR/lib))/*"
        ;;
    *)
        RELL_LIB=${BLOCKCHAIN_DIR}/lib/*
        ;;
esac

echo "Compiling contract..."
${RELL_JAVA:-java} -cp "$RELL_LIB" \
    net.postchain.rell.tools.runcfg.RellRunConfigGenKt \
    -d ${BLOCKCHAIN_DIR}/rell/src \
    -o ${BLOCKCHAIN_DIR}/target \
    ${BLOCKCHAIN_DIR}/rell/config/${ENVIRONMENT}/run.xml
echo "Done!"
echo ""

# copy contract 
echo "Copying compiled contract and configuration ..."
scp -i ~/.ssh/block_chain_rsa -o StrictHostKeyChecking=no ${BLOCKCHAIN_DIR}/target/blockchains/0/0.xml \
    ${BLOCKCHAIN_DIR}/rell/config/${ENVIRONMENT}/node-config.properties \
    $REMOTE_USER@$POSTCHAIN_NODE:$REMOTE_CONTRACT_DIR/${ENVIRONMENT}
echo "Done"
echo ""

# deploy contract
ssh -o StrictHostKeyChecking=no -T $REMOTE_USER@$POSTCHAIN_NODE -i ~/.ssh/block_chain_rsa <<-END
    POSTCHAIN_PIDS=\$(ps aux | grep AppKt | grep ${ENVIRONMENT} | grep -v grep | awk '{ print \$2 }')
    if [ ! -z "\$POSTCHAIN_PIDS" ]; then
        echo "Stopping postchain node..."
        kill "\$POSTCHAIN_PIDS"
        echo "Done"
        echo ""
    fi

    echo "Deleting database ..."
    ${REMOTE_POSTCHAIN_BIN_DIR}/postchain.sh wipe-db -nc ${REMOTE_CONTRACT_DIR}/${ENVIRONMENT}/node-config.properties
    echo "Done"
    echo ""

    echo "Adding blockchain ..."
    ${REMOTE_POSTCHAIN_BIN_DIR}/postchain.sh add-blockchain -cid 0 -bc ${REMOTE_CONTRACT_DIR}/${ENVIRONMENT}/0.xml -nc ${REMOTE_CONTRACT_DIR}/${ENVIRONMENT}/node-config.properties
    echo "Done"
    echo ""

    echo "Starting postchain node ..."
    nohup ${REMOTE_POSTCHAIN_BIN_DIR}/postchain.sh run-node -cid 0 -nc ${REMOTE_CONTRACT_DIR}/${ENVIRONMENT}/node-config.properties > /dev/null 2> /dev/null < /dev/null &
    echo "Done"
    echo ""
END

BRID=$(cat ${BLOCKCHAIN_DIR}/target/blockchains/0/brid.txt)
echo "Blockchain RID: $BRID"

echo "Checking if Vault config needs to be updated"

npm i

rm -rf $VAULT_LOCAL_DIR
git clone --branch $VAULT_BRANCH git@bitbucket.org:chromawallet/wallet.git $VAULT_LOCAL_DIR
node jenkins_update_vault_rid.js $VAULT_LOCAL_DIR/$VAULT_CONFIG_FILE $NODE_URL $BRID

cd $VAULT_LOCAL_DIR

if ! git diff-index --quiet HEAD; then
  git add $VAULT_CONFIG_FILE
  git commit -m "Jenkins update of MNA $NODE_URL RID"
  git push origin $VAULT_BRANCH
  echo "Updated Vault config"
else
  echo "No changes to commit in Vault"
fi

rm -rf $VAULT_LOCAL_DIR
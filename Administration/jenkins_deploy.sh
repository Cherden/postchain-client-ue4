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

case "$ENVIRONMENT" in
    environment/dev|environment/test|environment/stage|environment/release)
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
BLOCK_HEIGHT=$(curl ${NODE_URL}_debug | jq -r '.blockchain[0].height')
DEPLOY_HEIGHT=$((BLOCK_HEIGHT + 20))

echo "Deploying contract at $DEPLOY_HEIGHT"
ssh -o StrictHostKeyChecking=no -T $REMOTE_USER@$POSTCHAIN_NODE -i ~/.ssh/block_chain_rsa <<-END
  ${REMOTE_POSTCHAIN_BIN_DIR}/postchain.sh add-configuration --allow-unknown-signers -bc $REMOTE_CONTRACT_DIR/${ENVIRONMENT}/0.xml -h $DEPLOY_HEIGHT -cid 0 -nc ${REMOTE_CONTRACT_DIR}/${ENVIRONMENT}/node-config.properties 
END

echo "Blockchain RID:"
cat ${BLOCKCHAIN_DIR}/target/blockchains/0/brid.txt
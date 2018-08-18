#!/bin/bash

if [[ $# -ne 2 ]]; then
	echo "USAGE: build_release.sh <Account Name> <Contract Path> from within the directory of "eosio/eos-dev" docker container"
	exit 1
fi

ACCOUNT=$1
CONTRACT_PATH=$2
CONTRACT_NAME=${CONTRACT_PATH##*/}


#create alias for cleos and eosiocpp
alias cleos='docker-compose -f docker-compose-local-eosio1.0.yaml exec keosd /opt/eosio/bin/cleos -u http://nodeosd:8888 --wallet-url http://localhost:8900'

alias eosiocpp='docker-compose -f docker-compose-local-eosio1.0.yaml exec keosd /opt/eosio/bin/eosiocpp'

#compile for contracts
eosiocpp -o ${CONTRACT_PATH}/${CONTRACT_NAME}.wast ${CONTRACT_PATH}/${CONTRACT_NAME}.cpp && 
eosiocpp -g ${CONTRACT_PATH}/${CONTRACT_NAME}.abi ${CONTRACT_PATH}/${CONTRACT_NAME}.cpp && 

#deploy contract, defaults to 'account@active' permission
cleos set contract ${ACCOUNT} ${CONTRACT_PATH}


#!/bin/bash

if [[ $# -ne 2 ]]; then
	echo "USAGE: build_release.sh <Account Name> <Contract Name> from within the directory of "eosio/eos-dev" docker container"
	exit 1
fi

ACCOUNT=$1
CONTRACT=$2

#create alias for cleos and eosiocpp
alias cleos='docker-compose -f docker-compose-local-eosio1.0.yaml exec keosd /opt/eosio/bin/cleos -u http://nodeosd:8888 --wallet-url http://localhost:8900'

alias eosiocpp='docker-compose -f docker-compose-local-eosio1.0.yaml exec keosd /opt/eosio/bin/eosiocpp'

#compile for contracts
eosiocpp -o ${CONTRACT}/${CONTRACT}.wast ${CONTRACT}/${CONTRACT}.cpp && 
eosiocpp -g ${CONTRACT}/${CONTRACT}.abi ${CONTRACT}/${CONTRACT}.cpp && 

#deploy contract, defaults to 'account@active' permission
cleos set contract ${ACCOUNT} ${CONTRACT}


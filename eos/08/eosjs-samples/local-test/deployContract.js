const eosjs_config = require('../eosjs_config');
const fs = require('fs');

const eos = eosjs_config.eos;


const wasm = fs.readFileSync(`../../docker/appdemo/appdemo.wasm`);
const abi = fs.readFileSync(`../../docker/appdemo/appdemo.abi`);

// async
(async function () {

  // Options may be provided after parameters.
  options = {
    authorization: 'eosio@active',
    broadcast: true,
    sign: true
  };

  // Publish contract to the block chain net
  await eos.setcode('eosio', 0, 0, wasm, options);
  await eos.setabi('eosio', JSON.parse(abi), options);

  // Fetch contract info
  console.log('contract deployed:', await eos.contract('eosio'));

})().catch(console.error);




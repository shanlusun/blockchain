const eosjs_config = require('../eosjs_config');

const eos = eosjs_config.eos;


// async
(async function () {

  const code = await eos.getCode('eosio');
  const abi = await eos.getAbi('eosio');
  const actions = await eos.getActions('eosio', 0, 5); //pos和offset是指：从第pos条记录开始获取offset条Actions

  console.log('eosio code:', code);
  console.log('eosio abi:', abi);
  console.log('eosio actions:', actions);

})().catch(console.error);
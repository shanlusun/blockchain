const eosjs_config = require('../eosjs_config');

const eos = eosjs_config.eos;


// async
(async function () {

  // Prepare headers
  const expireInSeconds = 60 * 60; // 1 hour

  //eos = Eos(/* {httpEndpoint: 'https://..'} */)

  const info = await eos.getInfo({});
  const chainDate = new Date(info.head_block_time + 'Z'); // Don't use `new Date` in production
  let expiration = new Date(chainDate.getTime() + expireInSeconds * 1000);
  expiration = expiration.toISOString().split('.')[0];

  const block = await eos.getBlock(info.last_irreversible_block_num);

  const transactionHeaders = {
    expiration,
    ref_block_num: info.last_irreversible_block_num & 0xFFFF,
    ref_block_prefix: block.ref_block_prefix
  };

  // OFFLINE (bring `transactionHeaders`)
  // Options may be provided after parameters.
  options = {
    authorization: 'eosio@active',
    broadcast: true,
    sign: true,
    transactionHeaders
  };

  // All keys in keyProvider will sign.
  //eos = Eos({httpEndpoint: null, chainId, keyProvider, transactionHeaders});

  let callme = await eos.transaction(
      {
        // ...headers,
        //transactionHeaders,
        actions: [
          {
            account: 'eosio',
            name: 'callme',
            authorization: [{
              actor: 'eosio',
              permission: 'active'
            }],
            data: {
              account: 'eosio'
            }
          }
        ]
      },
      // options -- example: {broadcast: false}
      //options
  );

  console.log('callme:', callme);
  let transferTransaction = callme.transaction;

  let processedTransaction = await eos.pushTransaction(transferTransaction);
  console.log('processedTransaction:', processedTransaction);
})().catch(console.error);
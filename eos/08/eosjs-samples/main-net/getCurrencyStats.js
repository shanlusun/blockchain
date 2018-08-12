const eosjs_config = require('../eosjs_config');

const eos = eosjs_config.eos;


// Promise
eos.getCurrencyStats('eosio.token', 'EOS')
    .then(result => console.log(result))
    .catch(error => console.error(error));

// callback
eos.getCurrencyStats('eosio.token', 'EOS',
    (error, result) => console.log(error, result));

// Parameters object
eos.getCurrencyStats({code: 'eosio.token', symbol: 'EOS'})
    .then(console.log);
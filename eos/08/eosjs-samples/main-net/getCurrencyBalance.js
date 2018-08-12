const eosjs_config = require('../eosjs_config');

const eos = eosjs_config.eos;

// Promise
eos.getCurrencyBalance('eosio.token', 'eosio', 'EOS')
    .then(result => console.log('eosio:', result))
    .catch(error => console.error(error));

// callback
eos.getCurrencyBalance('eosio.token', 'eos', 'EOS',
    (error, result) => console.log('eos:', error, result));

// Parameters object
eos.getCurrencyBalance({account: 'eosdacserver', code: 'eosio.token', symbol: 'EOS'})
    .then(result => console.log('eosdacserver:', result));
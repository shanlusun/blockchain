const eosjs_config = require('../eosjs_config');

const eos = eosjs_config.eos;

// Promise
eos.getAccount('eosio')
    .then(result => console.log(result))
    .catch(error => console.error(error));

// callback
eos.getAccount('eosdacserver', (error, result) => console.log(error, result));

// Parameters object
eos.getAccount({account_name: 'eos42freedom'})
    .then(result => {
      console.log(result);
      console.log('eos42freedom permissions:', JSON.stringify(result.permissions[0].required_auth));
      console.log('eos42freedom permissions:', JSON.stringify(result.permissions[1].required_auth));
    })
    .catch(error => console.error(error));

// async
(async function () {
  console.log('async:', await eos.getInfo({}));

  console.log('accounts controlled by eosio:', await eos.getControlledAccounts('eosio'));
})().catch(console.error);
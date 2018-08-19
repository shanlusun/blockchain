const eosjs_config = require('../eosjs_config');

const eos = eosjs_config.eos;


const pubKey = 'EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV';

// async
async function newAccount() {

  // call two actions in the same transaction
  await eos.transaction(tr => {
    tr.newaccount({
      creator: 'eosio',
      name: 'hello',
      owner: pubKey,
      active: pubKey
    });

    tr.newaccount({
      creator: 'eosio',
      name: 'token',
      owner: pubKey,
      active: pubKey
    });

  });

  console.log('new account info:', await eos.getAccount('hello'));
  console.log('new account info:', await eos.getAccount('token'));

  console.log('accounts controlled by eosio:', await eos.getControlledAccounts('eosio'));
}

module.exports = {
  newAccount
};
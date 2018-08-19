const eosjs_config = require('../eosjs_config');
const newAcc = require('./newAccount');

const eos = eosjs_config.eos;

const pubKey = 'EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV';
const newPubKey = 'EOS7UN5ZY6WYpVhjkjPG4bh5rQxHgAeFKnjLBNok22cATD82JPjai';


async function getNewPermissions(accountName) {
  const account = await eos.getAccount(accountName);
  const perms = JSON.parse(JSON.stringify(account.permissions));// clone
  for(const perm of perms) {
    for(const key of perm.required_auth.keys) {
      if(key.key === pubKey) {
        key.key = newPubKey;
      }
    }
  }
  return perms;
}

async function updateAuths() {
  let affectedAccounts = (await eos.getKeyAccounts(pubKey)).account_names
  console.info('Affected account(s):', JSON.stringify(affectedAccounts))
  if(affectedAccounts.length === 0) {
    return
  }
  if(affectedAccounts.length > 1) {
    if(JSON.stringify(affectedAccounts.sort()) === '["hello","token"]') {
      affectedAccounts = ['hello']
    } else {
      throw new Error('Fix only one account at a time:', affectedAccounts)
    }
  }
  const accountName = affectedAccounts[0];
  const perms = await getNewPermissions(accountName);
  console.log('New permissions =>', JSON.stringify(perms));
  const updateAuthResult = await eos.transaction(tr => {
    for(const perm of perms) {
      tr.updateauth({
        account: accountName,
        permission: perm.perm_name,
        parent: perm.parent,
        auth: perm.required_auth
      }, {authorization: `${accountName}@owner`})
    }
  });
  console.log('Success =>', JSON.stringify(updateAuthResult));
}

(async function () {
  await newAcc.newAccount();
  console.log('new accounts created: hello, token');
  await updateAuths();
  console.log('updated auth of account: hello');

})().catch(console.error);


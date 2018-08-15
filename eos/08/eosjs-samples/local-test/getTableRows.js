const eosjs_config = require('../eosjs_config');

const eos = eosjs_config.eos;

// 已经弃用
eos.getTableRows({
  code:'CONTRACT_NAME',
  scope:'SCOPE_ACCOUNT (Normally contract)',
  table:'TABLE_NAME',
  json: true,
}).then(function(res) {
  console.log(res);
});

const eosjs_config = require('../eosjs_config');

const eos = eosjs_config.eos;


eos.contract("hello").then(appdemo => {  //appdemo随便起的变量名
  appdemo.callme('hello', {              //callme是方法名, 'eosio'是该eosio合约方法的参数
    authorization: ['hello']      //eosio是建立该合约的用户
  }).then(result => {
    console.log(result);
  });
});
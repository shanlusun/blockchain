const Eos = require('eosjs');

const httpEndpoint = process.env.BP || 'https://api.eosnewyork.io';
const chainId = process.env.ID || 'aca376f206b8fc25a6ed44dbdc66547c36c6c33e3a119ffbeaef943642f0e906';
const key = process.env.PRI_KEY || '5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3';

const config = {
  // private keys used to request net
  keyProvider: [key], // 配置私钥字符串

  httpEndpoint: httpEndpoint, // mainNet bp endpoint
  chainId: chainId, // 通过cleos get info可以获取chainId

  expireInSeconds: 60,
  broadcast: true,
  debug: false,
  sign: true,
  authorization: null // 该参数用于在多签名情况下，识别签名帐号与权限,格式如：account@permission

  // transactionHeaders: (expireInSeconds, callback) => {
  //   callback(null/*error*/, headers) //手动设置交易记录头，该方法中的callback回调函数每次交易都会被调用
  // },

};

const eos = Eos(config);

module.exports = {
  eos,
  key
};
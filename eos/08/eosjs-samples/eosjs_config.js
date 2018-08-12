const Eos = require('eosjs');

const httpEndpoint = process.env.BP || 'https://api.eosnewyork.io';
const chainId = process.env.ID || 'aca376f206b8fc25a6ed44dbdc66547c36c6c33e3a119ffbeaef943642f0e906';
const key = process.env.PRI_KEY || '5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3';

const config = {
  // private keys used to request net
  keyProvider: [key], // WIF string or array of keys..
  // mainNet bp endpoint
  httpEndpoint: httpEndpoint,
  // mainNet chainId
  chainId: chainId,
  expireInSeconds: 60,
  broadcast: true,
  debug: false,
  sign: true,
};

const eos = Eos(config);

module.exports = {
  eos,
  key
};
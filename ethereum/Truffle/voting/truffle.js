// Allows us to use ES6 in our migrations and tests.
require('babel-register');
var HDWalletProvider = require("truffle-hdwallet-provider");
const fs = require("fs");

var infura_apikey = "CwrqJlxnjg8oxqS69pVi";
var mnemonic = fs.readFileSync("mnemonic").toString();

module.exports = {
  networks: {
    development: {
      host: 'localhost',
      port: 8545,
      network_id: '*', // Match any network id
      gas: 470000 // The global gas value will be used by default, limit: 4728379
    },
    ropsten: {
      provider: new HDWalletProvider(mnemonic, "https://ropsten.infura.io/" + infura_apikey),
      network_id: 3
    }
  },
  solc: {
    optimizer: {
      enabled: true,
      runs: 200
    }
  }
}
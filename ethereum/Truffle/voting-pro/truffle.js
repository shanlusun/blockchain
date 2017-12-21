// Allows us to use ES6 in our migrations and tests.
require('babel-register')

module.exports = {
  networks: {
    development: {
      host: 'localhost',
      port: 8545,
      network_id: '*', // Match any network id
      gas: 870000 // The global gas value will be used by default, limit: 4728379
    }
  },
  solc: {
    optimizer: {
      enabled: true,
      runs: 200
    }
  }
}
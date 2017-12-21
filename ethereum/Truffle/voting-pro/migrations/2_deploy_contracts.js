var Voting = artifacts.require("./Voting.sol");
module.exports = function(deployer) {
  deployer.deploy(Voting, 1000, web3.toWei('0.1', 'ether'), ['Rama', 'Nick', 'Jose']);
};

// 1000 is the total number of tokens for sale and the price of each token is set to 0.1 ether.

var Voting = artifacts.require("./Voting.sol");
module.exports = function(deployer) {
  deployer.deploy(Voting, ['Rama', 'Nick', 'Jose'], {gas: 419630});
};
/* As you can see above, the deployer expects the first argument to   be the name of the contract followed by constructor arguments. In our case, there is only one argument which is an array of
candidates. The third argument is a hash where we specify the gas required to deploy our code. The gas amount varies depending on the size of your contract.
*/
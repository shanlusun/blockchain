pragma solidity ^0.4.2;

import "truffle/Assert.sol";
import "truffle/DeployedAddresses.sol";
import "../contracts/Voting.sol";

contract TestVoting {

  function testInitialCandidateListDeployedContract() public {
    Voting vote = Voting(DeployedAddresses.Voting());

    uint8 expected = 0;
    bytes32 name = "Rama";

    Assert.equal(vote.totalVotesFor(name), expected, "Rama should have 0 vote initially");
  }
}

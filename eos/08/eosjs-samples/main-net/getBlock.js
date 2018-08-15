const eosjs_config = require('../eosjs_config');

const eos = eosjs_config.eos;


// Promise
eos.getBlock(11254000)
    .then(result => console.log(result))
    .catch(error => console.error(error));

// callback
eos.getBlock(11254000, (error, result) => console.log(error, result));

// Parameters object
eos.getBlock({block_num_or_id: 11254000})
    .then(console.log);
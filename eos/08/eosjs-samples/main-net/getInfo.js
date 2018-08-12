const eosjs_config = require('../eosjs_config');

const eos = eosjs_config.eos;


// An API with no parameters is invoked with an empty object or callback (avoids logging usage)
// Promise
eos.getInfo({})
    .then(result => console.log(result)); // @returns {Promise}

// callback
eos.getInfo((error, result) => { console.log(error, result) });

// async
(async function () {
  console.log('async:', await eos.getInfo({}));

})().catch(console.error);
const eosjs_config = require('../eosjs_config');

const eos = eosjs_config.eos;


// Promise
eos.getKeyAccounts('EOS5rgA6SenKsWJysTRwS6nek8uHbT7PjsDMY76sZkHEW74Htnc5p')
    .then(result => console.log(result))
    .catch(error => console.error(error));

// callback
eos.getKeyAccounts('EOS5rgA6SenKsWJysTRwS6nek8uHbT7PjsDMY76sZkHEW74Htnc5p',
    (error, result) => console.log(error, result));

// Parameters object
eos.getKeyAccounts({public_key: 'EOS8QoCJ5TcopvKTVW5trtRKeynK5KCdTJiwwcFfdn8FVWxrfnhnN'})
    .then(console.log);
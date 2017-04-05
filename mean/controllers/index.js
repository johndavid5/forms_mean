// Take code mostly from server.js...
var router = require('express').Router();

var bodyParser = require('body-parser');

router.use(require('../auth'))
router.use(require('../no-cache'));

router.use('/api/forms', require('./api/forms'));
router.use('/api/formTypes', require('./api/formTypes'));
//app.use('/api/sessions', require('./api/sessions'));
//app.use('/api/users', require('./api/users'));

// Static controllers for static content...
router.use('/', require('./static'))
// In case we're coming in via a reverse proxy mapped to /sec_forms...
router.use('/sec_forms', require('./static'))

module.exports = router;

var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('printManager', { title: 'Print Manager', printJob: 'Test.gcode' });
});

module.exports = router;
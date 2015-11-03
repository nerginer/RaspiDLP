var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  

	var fs = require("fs"),
	    path = require("path");


	var files = fs.readdirSync('./printDataDir');



	var fileDef= 'IMPORTANT: There is no gcode file in printDataDir use Job Creator to generate one';



	files.forEach(function (file) {
	        if (path.extname(file)=='.gcode'){
	        	
	        	fileDef= file;
	        	//Document.getElementById("buttonCreateJob").disabled = false; 
	        }
	       
	});



	res.render('printManager', { title: 'Print Manager', printJob: fileDef });
});

module.exports = router;
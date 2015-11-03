var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  
var fs = require("fs"),
    path = require("path");


var files = fs.readdirSync('./downloadDir');

var fileDef='';



files.forEach(function (file) {
        if (path.extname(file)=='.zip'){
        	
        	fileDef= file;
        	//Document.getElementById("buttonCreateJob").disabled = false; 
        }
        else {
        		
        	fileDef= 'IMPORTANT: There is no image zip file in downloadDir. Upload one to create print job';
        	//Document.getElementById("buttonCreateJob").disabled = true; 
        }
});


 

res.render('jobCreator', { title: 'Job Creator', printFile: fileDef });

});

module.exports = router;
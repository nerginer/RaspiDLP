var express = require('express');
var path = require('path');
var favicon = require('serve-favicon');
var logger = require('morgan');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');
var routes = require('./routes/index');
var upload = require('./routes/upload');
var printManager = require('./routes/printManager');
var jobCreator = require('./routes/jobCreator');
var meterials = require('./routes/meterials');
var printerProfile = require('./routes/printerProfile');


var PythonShell = require('python-shell');

var app = express();

var multer  = require('multer');
var done=false;



/*

//faye

var faye = require('faye');

var client = new faye.Client('http://localhost:8000/faye');

//This was missing from the documentation
client.connect();

var subscription = client.subscribe('/messages', function(message){
    console.log("Event Received");
    console.log(message);
})

*/


// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

// uncomment after placing your favicon in /public
//app.use(favicon(path.join(__dirname, 'public', 'favicon.ico')));
app.use(logger('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));



/*Configure the multer.*/

app.use(multer({ dest: './printDataDir/',
 rename: function (fieldname, filename) {
    return filename+Date.now();
  },
onFileUploadStart: function (file) {
  console.log(file.originalname + ' is starting ...')
},
onFileUploadComplete: function (file) {
  console.log(file.fieldname + ' uploaded to  ' + file.path)
  done=true;
}
}));

app.post('/upload',function(req,res){
  if(done==true){
    console.log(req.files);
    res.render('fileUploaded', { title: 'File Uploaded' });
    //res.end("File uploaded.");
  }
});

app.post('/createJob', function(req, res){
    var meterial = req.body.meterial;
    
    console.log('meterial: '+ meterial);
    
 
    var options = {
      mode: 'text',
      pythonPath: '/Library/Frameworks/Python.framework/Versions/2.7/Resources/Python.app/Contents/MacOS/Python',
      pythonOptions: ['-u'],
      scriptPath: '/Users/gnexlab_imac/code/dlp_raspi/RaspiDLP/Python/',
      args: [meterial]
    };
     
    PythonShell.run('gcodeGenerator.py', options, function (err, results) {
      if (err) throw err;
      // results is an array consisting of messages collected during execution 
      console.log('results: %j', results);
    });

    setTimeout(function() {
      res.statusCode = 302; 
      res.setHeader("Location", "/printManager");
      res.end();
    }, 3000);

    

});

app.post('/printManager', function(req, res){
   
    
 
    var options = {
      mode: 'text',
      pythonPath: '/Library/Frameworks/Python.framework/Versions/2.7/Resources/Python.app/Contents/MacOS/Python',
      pythonOptions: ['-u'],
      scriptPath: '/Users/gnexlab_imac/code/dlp_raspi/RaspiDLP/Python/',
      
    };
     
    PythonShell.run('printManager.py', options, function (err, results) {
      if (err) throw err;
      // results is an array consisting of messages collected during execution 
      console.log('results: %j', results);
    });

   

    

});




app.use('/', routes);
app.use('/upload', upload);
app.use('/printManager', printManager);
app.use('/jobCreator', jobCreator);
app.use('/meterials', meterials);
app.use('/printerProfile', printerProfile);


// catch 404 and forward to error handler
app.use(function(req, res, next) {
  var err = new Error('Not Found');
  err.status = 404;
  next(err);
});

// error handlers

// development error handler
// will print stacktrace
if (app.get('env') === 'development') {
  app.use(function(err, req, res, next) {
    res.status(err.status || 500);
    res.render('error', {
      message: err.message,
      error: err
    });
  });
}

// production error handler
// no stacktraces leaked to user
app.use(function(err, req, res, next) {
  res.status(err.status || 500);
  res.render('error', {
    message: err.message,
    error: {}
  });
});







module.exports = app;

//var async = require("async");

// read gcode for looper
/*
var fs = require('fs'),
    readline = require('readline');

var rd = readline.createInterface({
    input: fs.createReadStream('printDataDir/test.gcode'),
    output: process.stdout,
    terminal: false
});

rd.on('line', function(line) {

    
       
            if(line.indexOf(';<Slice>') > -1) {
              console.log('slice');
              usingItNow(myCallback,line);


            }
        
            
             
        

});

var myCallback = function(line) {
 if(line.indexOf(';<Delay>') > -1) {
      setTimeout(function() {
        console.log('delaying');
      }, 2000);
  }

};

var usingItNow = function(callback,line) {
  callback(line);
};


*/


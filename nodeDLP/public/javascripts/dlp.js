
var client = new Faye.Client('http://192.168.1.108:8000/faye');





client.subscribe('/status', function(message) {
  document.getElementById('status').innerHTML = message.text;
  //alert('Got a message: ' + message.text);

});



function playFunction() {
    console.log ( 'Published' );
    
    client.publish('/messages', {
  		text: 'play'
	});

}

function pauseFunction() {
    console.log ( 'Published' );
    
    client.publish('/messages', {
  		text: 'pause'
	});

}

function stopFunction() {
    console.log ( 'Published' );
    
    client.publish('/messages', {
  		text: 'stop'
	});

}


var client = new Faye.Client('http://localhost:8000/faye');






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

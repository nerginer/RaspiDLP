
var client = new Faye.Client('http://localhost:8000/faye');






function myFunction() {
    console.log ( 'Published' );
    
    client.publish('/messages', {
  		text: 'print'
	});

}

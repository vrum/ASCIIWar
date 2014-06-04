/*
 * :
 */
var asciiwar = {};
asciiwar.release = false;

console.log("release ", asciiwar.release);

asciiwar.play1v1 = function() {
  console.log('play1v1');
  $.ajax({
    type: "GET", 
    url: login.url + "play/" + login.username + "/" + login.token,
    success: function(d) {
      console.log(d);
      if(d.success) {
        console.log("cool");
      } else {
        console.log("pas cool");
      }
    },
    error: function() {
      console.log("error in play 1v1");
    }
  });
}
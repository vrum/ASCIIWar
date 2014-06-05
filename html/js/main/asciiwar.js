/*
 * :
 */
var asciiwar = {};
asciiwar.release = false;

console.log("release ", asciiwar.release);

if(asciiwar.release) 
  asciiwar.url = "https://citadelwar.com:8080/";
else
  asciiwar.url = "http://localhost:8080/";

console.log("login url ", asciiwar.url);

asciiwar.play1v1 = function() {
  console.log('play1v1');
  $.ajax({
    type: "GET", 
    url: asciiwar.url + "play1v1/" + login.username + "/" + login.token,
    success: function(d) {
      console.log(d);
      if(d.success) {
        $("#flash-login").html("<span class='success'>Launching...</span>");
        $.ajax({
          type: "GET", 
          url: asciiwar.url + "wait1v1/" + login.username + "/" + login.token,
          success: function(d) {
            console.log(d);
            if(d.success) {
              console.log("game name " + d['game-name'])
              $("#flash-login").html("<span class='success'>Playing...</span>");
              require('child_process').exec(
                '"bin/asciiwar.exe" ' +
                d['game-name'] + " " +
                d['player-count'] + " " +
                d['team-count']);
            } else {
              $.ajax(this);
              console.log("waiting...");
            }
          },
          error: function() {
            $("#flash-login").html("<span class='failure'>cannot connect to server</span>");
          }
        });
        console.log("cool");
      } else {
        $("#flash-login").html("<span class='failure'>Server error</span>");
      }
    },
    error: function() {
      $("#flash-login").html("<span class='failure'>cannot connect to server</span>");
    }
  });
}
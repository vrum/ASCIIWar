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

asciiwar.play = function(what) {
  console.log('play' + what);
  $.ajax({
    type: "GET", 
    url: asciiwar.url + "play" + what + "/" + login.username + "/" + login.token,
    success: function(d) {
      console.log(d);
      if(d.success) {
        $("#flash-login").html("<span class='success'>waiting players...</span>");
        $.ajax({
          type: "GET", 
          url: asciiwar.url + "wait" + what + "/" + login.username + "/" + login.token,
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
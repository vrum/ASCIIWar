/*
 * :
 */
var asciiwar = {};
asciiwar.release = true;

console.log("release ", asciiwar.release);

if(asciiwar.release) 
  asciiwar.url = "https://citadelwar.com:8080/";
else
  asciiwar.url = "http://localhost:8080/";

console.log("login url ", asciiwar.url);

asciiwar.play = function(what) {
  if(!asciiwar.playing) {
    asciiwar.playing = true;
    console.log('play' + what);
    $("#flash-login").html("<span class='success'>waiting players...</span>");
    $.ajax({
      type: "GET", 
      url: asciiwar.url + "play" + what + "/" + login.username + "/" + login.token,
      success: function(d) {
        console.log(d);
        if(d.success) {
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
                  d['team-count'],
                  function (error, stdout, stderr) {
                    asciiwar.playing = false;
                    $("#flash-login").html("<span class='success'></span>");
                  });
              } else {
                console.log("Waiting...");
                $.ajax(this);
              }
            },
            error: function() {
              asciiwar.playing = false;
              $("#flash-login").html("<span class='failure'>cannot connect to server</span>");
            }
          });
        } else {
          asciiwar.playing = false;
          if(d['need-login']
          || d['need-register'])
            $("#flash-login").html("<span class='warn'>Please log-in.</span>");
          else
            $("#flash-login").html("<span class='failure'>Server error.</span>");
        }
      },
      error: function() {
        asciiwar.playing = false;
        $("#flash-login").html("<span class='failure'>cannot connect to server</span>");
      }
    });
  }
}
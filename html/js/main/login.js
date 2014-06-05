var login = {};

login.min_length = 6;

login.check = function(username, password) {
  if(username == ""
  || password == "") {
    $("#flash-login").html("<span class='failure'>username and password cannot be blank</span>");
    return false;
  }
  if(username.match(/;/) != null
  || password.match(/;/) != null) {
    $("#flash-login").html("<span class='failure'>';' is not allowed</span>");
    return false;
  }
  if(password.length < login.min_length) {
    $("#flash-login").html("<span class='failure'>password must contain at least 6 characters.</span>");
    return false;
  }
  return true;
}

login.register = function() {
  console.log("register");
  var username = $("#login-username").val();
  var password = $("#login-password").val();
  if(login.check(username, password)) {
    $.ajax({
      type: "GET", 
      url: asciiwar.url + "register/" + username + "/" + password,
      success: function(d) {
        console.log(d);
        if(d.success) {
          login.username = username;
          login.token = d.token;
          $("#login").remove();
          $("#top").append(" \
            <div id='play'> \
            <div class='hero-unit'> \
              <div> <a class='btn btn-small' href='javascript:asciiwar.play1v1()'>Play 1v1</a> \
              <div id='flash-login'></div> \
            </div> \
            </div> \
            </div>");
          $("#flash-login").html("<span class='success'>Success</span>");
        } else 
        if(d["already-exist?"]) {
          $("#flash-login").html("<span class='warn'>Username already exists</span>");
        }
        else 
          $("#flash-login").html("<span class='failure'>Error</span>");
      },
      error: function() {
        $("#flash-login").html("<span class='failure'>cannot connect to server</span>");
      }
    });
  }
}

login.login = function() {
  console.log("login");
  var username = $("#login-username").val();
  var password = $("#login-password").val();
  if(login.check(username, password)) {
    $.ajax({
      type: "GET", 
      url: asciiwar.url + "login/" + username + "/" + password,
      success: function(d) {
        console.log(d);
        if(d.success) {
          login.username = username;
          login.token = d.token;
          $("#login").remove();
          $("#top").append(" \
            <div id='play'> \
            <div class='hero-unit'> \
              <div> <a class='btn btn-small' href='javascript:asciiwar.play1v1()'>Play 1v1</a> \
              <div id='flash-login'></div> \
            </div> \
            </div> \
            </div>");
          $("#flash-login").html("<span class='success'>Success</span>");
        } else {
          $("#flash-login").html("<span class='failure'>incorrect username or password</span>");
        }
      },
      error: function(d) {
        $("#flash-login").html("<span class='failure'>cannot connect to server</span>");
      }
    });
  }
}
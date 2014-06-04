var login = {};

login.min_length = 6;
if(asciiwar.release) 
  login.url = "https://citadelwar.com:8080/";
else
  login.url = "http://localhost:8080/";

console.log("login url ", login.url);

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

login.login = function() {
  console.log("login");
  var username = $("#login-username").val();
  var password = $("#login-password").val();
  if(login.check(username, password)) {
    $.ajax({
      type: "GET", 
      url: login.url + "login/" + username + "/" + password,
      success: function(d) {
        if(d.success) {
          login.token = d.token;
          $("#flash-login").html("<span class='success'>Success</span>");
        } else {
          $("#flash-login").html("<span class='failure'>Error</span>");
        }
      },
      error: function() {
        $("#flash-login").html("<span class='failure'>incorrect username or password</span>");
      }
    });
  }
}

login.register = function() {
  console.log("register");
  var username = $("#login-username").val();
  var password = $("#login-password").val();
  if(login.check(username, password)) {
    $.ajax({
      type: "GET", 
      url: login.url + "register/" + username + "/" + password,
      success: function(d) {
        console.log(d);
        if(d.success) {
          login.token = d.token;
          $("#flash-login").html("<span class='success'>Success</span>");
        } else 
        if(d["already-exist?"]) {
          $("#flash-login").html("<span class='warn'>Username already exists</span>");
        }
        else 
          $("#flash-login").html("<span class='failure'>Error</span>");
      },
      error: function() {
        $("#flash-login").html("<span class='failure'>internal error</span>");
      }
    });
  }
}
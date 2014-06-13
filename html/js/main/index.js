/*
 * :
 */
$(function(){
	$.ajaxSetup();
  var gui = require('nw.gui');
  var win = gui.Window.get();
  $("*").keydown(function(e) {
    if(e.keyCode === 123) {
      gui.Window.get().showDevTools();
    }
    /*if(e.keyCode === 27) {
      win.close();
    }*/
    if(e.keyCode == 13){
      console.log("click");
      window.location.href = $('#login-btn').attr("href");
    }
  });
  $("#login-username").focus();
  win.on('close', function() {
    login.logoff();
    this.close(true);    
  });
});
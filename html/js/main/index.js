/*
 * :
 */
$(function(){
	$.ajaxSetup();
  var gui = require('nw.gui');
  $("*").keydown(function(e) {
    if(e.keyCode === 123) {
      gui.Window.get().showDevTools();
    }
    if(e.keyCode === 27) {
      gui.App.quit();
    }
    if(e.keyCode == 13){
      console.log("click");
      window.location.href = $('#login-btn').attr("href");
    }
  });
  $("#login-username").focus();
});
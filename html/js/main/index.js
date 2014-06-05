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
  });
});
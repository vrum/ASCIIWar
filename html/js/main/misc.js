/*
 * Misc:
 */
var setCookie = function(name, value){
	document.cookie=name + "=" + escape(value);
}

var getCookie = function(name){
	var i,
		x,
		y,
		ARRcookies = document.cookie.split(";");
		
	for( i = 0 ; i < ARRcookies.length ; i++ ) {
		x = ARRcookies[i].substr( 0, ARRcookies[i].indexOf("=") );
	  	y = ARRcookies[i].substr( ARRcookies[i].indexOf("=") + 1 );
	  	x = x.replace(/^\s+|\s+$/g,"");
	  	if( x == name ){
	    	return unescape(y);
	  	}
	}
	return null;
}

var assert = function(exp, msg) {
	if( !(exp) ) alert( 'assert: ' + msg );
}

function guidGenerator() {
	Math.seedrandom();
    var S4 = function() {
       return (((1+Math.random())*0x10000)|0).toString(16).substring(1);
    };
    return ""+(S4()+S4()+"-"+S4()+"-"+S4()+"-"+S4()+"-"+S4()+S4()+S4());
}

var getQueryStringObject = function () {
  var query_string = {};
  var query = window.location.search.substring(1);
  var vars = query.split("&");
  for (var i=0;i<vars.length;i++) {
    var pair = vars[i].split("=");
    if (typeof query_string[pair[0]] === "undefined") {
      query_string[pair[0]] = pair[1];
    } else if (typeof query_string[pair[0]] === "string") {
      var arr = [ query_string[pair[0]], pair[1] ];
      query_string[pair[0]] = arr;
    } else {
      query_string[pair[0]].push(pair[1]);
    }
  } 
    return query_string;
}

var urlRegExp = /(http(s)?:\/\/)?(([\w-]+\.)+)([\w-]+)(\/[\w- ;,./?%&=]*)?/

var getHost = function(url) {
	var matches = url.match(urlRegExp);
	if( matches != null && matches.length > 5) 
    	return matches[3] + matches[5];
    return "";
}

var getDomainName = function(url) {
  var matches = url.match(urlRegExp);
  if( matches != null && matches.length > 5) 
      return matches[4];
    return "";
}

function isEmpty(obj) {
    if (obj === null || obj === undefined) return true;
    if (obj.length && obj.length > 0)    return false;
    if (obj.length === 0)  return true;
    for (var key in obj) {
        return false;
    }
    return true;
}

function htmlForTextWithEmbeddedNewlines(text) {
    var htmls = [];
    var lines = text.split(/\n/);
    var tmpDiv = jQuery(document.createElement('div'));
    for (var i = 0 ; i < lines.length ; i++) {
        htmls.push(tmpDiv.text(lines[i]).html());
    }
    return htmls.join("<br>");
}

function trim( str ) {
  return str.replace(/^\s\s*/, '').replace(/\s\s*$/, '');
}
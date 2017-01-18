window.onload = function () {
	var url = document.location.href, info = url.split('?')[1],
		info_de = decodeURIComponent(info),
		params = info_de.split('&'),
		data = {}, tmp;
	for (var i = 0, l = params.length; i < l; i++) {
		tmp = params[i].split('=');
		data[tmp[0]] = tmp[1];
	}
	document.getElementById('username').value = data.username;
	document.getElementById('password').value = data.password;
	document.getElementById('nickname').value = data.nickname;
	document.getElementById('picname').value = data.picname;
}

$(document).ready(function(){
  $("form#editForm").submit(function() { // editForm is submitted


var username = $('#username').attr('value'); // get name
var password = $('#password').attr('value'); // get password
var nickname = $('#nickname').attr('value'); // get name
var picname = $('#picname').attr('value'); // get name

if (username && password && nickname) { // values are not empty
    $.ajax({
        type: "POST",
        url: "/update.cgi", // URL of the Perl script

        // send username and name as parameters to the Perl script
        data: "username=" + username + "&password=" + password + "&nickname=" + nickname + "&picname=" + picname,

        // script call was *not* successful
        error: function() { 
            alert("script call was not successful");
        }, 

        // script call was successful 
        // data should contain the string returned
        success: function(ret_data) {
        	var info = ret_data;
            alert("Update Success!")
			url = 'editprofile.html?' 
					+ encodeURIComponent(info);

			document.location.href = url;
        }
    });   
}

else {
  $('div#editResult').text("Enter your information");
  $('div#editResult').addClass("error");
}

$('div#editResult').fadeIn();
return false;
  });
});


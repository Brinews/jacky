$(document).ready(function(){
  $("form#loginForm").submit(function() { // loginForm is submitted


var username = $('#username').attr('value'); // get first name
var password = $('#password').attr('value'); // get name

if (username && password) { // values are not empty
    $.ajax({
        type: "POST",
        url: "/login.cgi", // URL of the Perl script

        // send username and password as parameters to CGI
        data: "username=" + username + "&password=" + password,

        // script call was *not* successful
        error: function() { 
            alert("script call was not successful");
        }, 

        // script call was successful 
        // ret_data should contain the string returned by the Perl script 
        success: function(ret_data){
			var info = ret_data;
            alert("Your info is: " + ret_data)
			url = 'editprofile.html?' 
					+ encodeURIComponent(info);

			document.location.href = url;
        }
    });   
}

else {
  $('div#loginResult').text("Enter your name and password");
  $('div#loginResult').addClass("error");
}

$('div#loginResult').fadeIn();
return false;
  });
});


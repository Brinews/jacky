
$(document).ready(function(){
  $("form#uploadForm").submit(function() { // editForm is submitted

  var picname = $('#localpic').attr('value'); // get name

  if (picname) { // values are not empty
    $.ajax({
        type: "POST",
        url: "/up.cgi", // URL of the CGI
        cache: false,
		data: new FormData($('#uploadForm')[0]),
		processData: false,
		contentType: false,

        // script call was *not* successful
        error: function() { 
            alert("script call was not successful");
        }, 

        // script call was successful 
        success: function(ret_data) {
			document.getElementById('picname').value = ret_data;
        }
    });   
  }
  return false;
  });
});


#!/usr/bin/perl -w

# written by andrewt@cse.unsw.edu.au October 2017
# as a starting point for COMP[29]041 assignment 2
# https://cgi.cse.unsw.edu.au/~cs2041/assignments/UNSWtalk/

use CGI qw/:all/;
use CGI::Carp qw/fatalsToBrowser warningsToBrowser/;

# main entry
sub main {
    # print start of HTML ASAP to assist debugging if there is an error in the script
    print page_header();

    # Now tell CGI::Carp to embed any warning in HTML
    warningsToBrowser(1);

    # define some global variables
    $students_dir = "dataset-medium";

    print student_page();

    # print footer
    print page_trailer();
}

#
# sort the post by id
# @param $a id1
# @param $b id2
#
sub numsort {
	my @name1 = split('/', $a);
	my @name2 = split('/', $b);

	$na1 = $name1[-1];
	$na2 = $name2[-1];

	$na1 =~ s/\.txt//;
	$na2 =~ s/\.txt//;

    # get id from text file name
	my @n1 = split('-', $na1);
	my @n2 = split('-', $na2);

	$n1[1] <=> $n2[1]
}

#
# @param string
# @return string remove header and tail space
#
sub trim {
	my ($string) = @_;

    # replace the space in the head and tail
	$string =~ s/^\s+//;
	$string =~ s/\s+$//;

	return $string;
}

#
# @param student filename
# @return student name
#
sub getName {
	my ($filename) = @_;

	open my $fd, $filename or die "open friend file $filename failed.";

	my %stuhash;

    # open file and read full_name
	while (<$fd>) {
		chomp;
		my $line = $_;

		my ($key, $value) = split(':', $line);

		$key = trim($key);
		$value = trim($value);

        # store key and value
		$stuhash{lc($key)} = $value;
	}

	close $fd;

	return $stuhash{"full_name"};
}

# 
# @param student file name
# @return student password
#
sub getPassword {
	my ($filename) = @_;

	open my $fd, $filename or die "open friend file failed.";

	my %stuhash;

	while (<$fd>) {
		chomp;
		my $line = $_;

        # split by :
		my ($key, $value) = split(':', $line);

        # get the key and value 
		$key = trim($key);
		$value = trim($value);

		$stuhash{lc($key)} = $value;
	}

	close $fd;

	return $stuhash{"password"};
}

#
# @param student id
# @param friend id
# @param operation, 0: add, 1:del
# 
sub add_friend {
	my ($uid, $fid, $op) = @_;

	if ($uid eq $fid) {
		return;
	}

	my $filename = "$students_dir/$uid/student.txt";

	open my $fd, $filename or die "open friend file $filename failed.";

	my %stuhash;

    # read student information
	while (<$fd>) {
		chomp;
		my $line = $_;

		my ($key, $value) = split(':', $line);

		$key = trim($key);
		$value = trim($value);

		$stuhash{lc($key)} = $value;
	}

	close $fd;

	my @frds = ();
	my $fs = $stuhash{'friends'};

    # update student file by adding or removing friends list
	open(STU, ">", "$students_dir/$uid/student.txt") or die $!;
	#open(STU, ">", "/tmp/$uid-student.txt") or die $!;

	if ($stuhash{'friends'} =~ /\((.*)\)/) {
		my @fds = split(',', $1);

		my $found = 0;

		foreach my $f (@fds) {
			$f = trim($f);

			if (($op == 0) && ($f eq $fid)) {
				next; # need delete
			}
			if (($op == 1) && ($f eq $fid)) {
				$found = 1; # already exist
			}
			push(@frds, $f);
		}

		if (($op == 1) && ($found == 0)) {
			push(@frds, $fid);
		}
	}

	# update friends list

	foreach my $k (keys %stuhash) {
		next if ($k eq "friends");

		print STU $k.": ";
		print STU $stuhash{$k}."\n";
	}

    # append friends list to student file
	if (@frds > 0) {
		print STU "friends: (";
		my $first = 0;
		foreach my $fi (@frds) {
			if ($first != 0) {
				print STU ", ";
			}
			print STU $fi;
			$first = 1;
		}
		print STU ")\n";
	}

	close(STU);
}

# 
# post new message
# @param content of message
# @param uid student id
# @param post id
#
sub post_message {
	my ($content, $uid, $idno) = @_;

    # get local time of now
	my ($sec, $min, $hour, $mday, $mon, $year, $wday, $yday, $isdst)=localtime();

	$year += 1900;

	my $ti = "$year-$mon-$mday"."T"."$hour:$min:$sec+0000";

    # create post file
	open (NEW, ">", "$students_dir/$uid/$idno.txt") or die $!;
    # add file content
	print NEW "from: $uid\n";
	print NEW "message: $content\n";
	print NEW "time: $ti\n";

	close(NEW);
}

#
# @param level 0: comment, 1: reply
# @Param post number id
# @Param posts of all
# @param row the output html
# @param mkey search key of posts
# @param id current student id
# @param page page number
# @param idx current posts number
#
sub addPostRecursively {
	my ($level, $postnoRef, $postsRef, $rowRef, $mkey, $id, $page, $idx) = @_;

	my @posts1 = @$postsRef;
	my @posts;

	my %posthash;

    # get all posts's comment
	foreach my $pt (@posts1) {
		my @filename = split('/', $pt);
		my $no = $filename[-1];

		if ($no =~ /^$postnoRef\-\d+\.txt$/) {
			push(@posts, $pt);
		}
	}

    # add posts by id order and filter by search key
	foreach my $post (sort numsort @posts) {
		my @filename = split('/', $post);
		my $no = $filename[-1];

        # match the reply
		if ($no =~ /^$postnoRef\-\d+\.txt$/) {
			open my $po, $post or die "open post failed.";

			my %phash;
			$no =~ s/\.txt//;

            # read out posts
			while (<$po>) {
				chomp;
				my $line = $_;
				my ($key, $value) = split(':', $line);

				$phash{lc($key)} = trim($value);
			}

			close $po;

			# filter by key
			if ($phash{"message"} !~ /$mkey/) {
				next;
			}

			$idx += 1;

            # page handle
			if ($idx < $page*16 || $idx > ($page+1)*16) {
				next;
			}

			if ($level == 0) {
				$rowRef = $rowRef . "<tr style='background:#ccc;font-size:13px;'>";
			} elsif ($level == 1) {
				$rowRef = $rowRef . "<tr style='background:#99cc33;font-size:11px;'>";
			}

            # display control
			$rowRef = $rowRef . "<td>No:$idx, Author:</td><td>";
			my $fid = trim($phash{"from"});
			my $name = getName("$students_dir/$fid/student.txt");
			$rowRef = $rowRef . $phash{"from"} ;
			$rowRef = $rowRef . "< $name >";
			$rowRef = $rowRef."</td><td>Time:</td><td>$phash{'time'}</td></tr>";

			if ($level == 0) {
				$rowRef = $rowRef . "<tr style='color:#ccc;font-size:13px;'>";
			} elsif ($level == 1) {
				$rowRef = $rowRef . "<tr style='color:#99cc33;font-size:11px;'>";
			}

			$rowRef = $rowRef."<td>Content:</td><td colspan=3>";
			$rowRef = $rowRef. $phash{"message"};
			$rowRef = $rowRef . "<a href='?userid=$id'>Reply</a></td></tr>";

            # add the post recursivly
			($rowRef, $idx) = addPostRecursively($level+1, $no, \@posts1, $rowRef, $mkey, $id, $page, $idx);
		}
	}

	return ($rowRef, $idx);
}

# 
# @param register id
#
sub do_register {
	my ($regid) = @_;

	#mkdir "/tmp/$regid";
	#open(N, ">", "/tmp/$regid/student.txt");
	mkdir "$students_dir/$regid";
    # create user file
	open(N, ">", "$students_dir/$regid/student.txt");

	my $pwd = param('regpwd') || '';
	my $name = param('name') || '';
	my $email = param('email') || '';
	my $birth = param('birth') || '';

	print N "zid: $regid\n";
	print N "password: $pwd\n";
	print N "full_name: $name\n";
	print N "birthday: $birth\n";

	close(N);
}

#
# Show unformatted details for student "n".
# Increment parameter n and store it as a hidden variable
#
sub student_page {
    my $n = param('n') || 0;
    my @students = sort(glob("$students_dir/*"));

	my $validlogin = 0;

	my $id = param('userid');
	my $pwd = param('passwd') || '';

	my $reg = param('register') || 0;

	my @tno;
	my $tpwd;

	my $regid = param('regid') || '';
	if ($regid ne '') {
		do_register($regid);
	}

	if ($reg == 1) {
		return <<eof;
<form method="POST" action="">
<table>
<tr>
<td>LoginID:</td>
<td>
<input type="textbox" name="regid" value="">
</td></tr>
<tr><td>Password:</td><td>
<input type="password" name="regpwd" value="">
</td></tr>
<tr><td>FullName:</td><td>
<input type="textbox" name="name" value="">
</td></tr>
<tr><td>Email:</td><td>
<input type="textbox" name="email" value="">
</td></tr>
<tr><td>Birthday:</td><td>
<input type="textbox" name="birth" value="">
<tr><td> </td></tr>
<tr><td colspan=2>
<input type="submit" value="Register" class="unswtalk_button">
</td></tr>
</table>
</form>
eof

	}

    # login session control
	if (($id ne '') && (-e "/tmp/$id")) {
		$validlogin = 1;
	}

    # logout control
	my $lout = param('logout') || '0';

	if ($lout eq "1") {
		$validlogin = 0;
		unlink "/tmp/$id";
	}

    # check user login information
	if (($id ne '') && ($pwd ne '') && ($validlogin == 0)) {
		foreach my $one (@students) {
			@tno = split('/', $one);
			if ($tno[1] eq $id) {
				$tpwd = getPassword("$one/student.txt");
				if ($tpwd eq $pwd) {
					$validlogin = 1;
					last;
				}
			}

			$n += 1;
		}
	}

    # need login again
	if ($validlogin == 0) {
		return <<eof;
<form method="POST" action="">
<table>
<tr>
<td>LoginID:</td>
<td>
<input type="textbox" name="userid" value="$tno[1]">
</td></tr>
<tr><td>Password:</td><td>
<input type="password" name="passwd" value="$tpwd">
</td></tr>
<tr><td> </td></tr>
<tr><td colspan=2>
<input type="submit" value="Login" class="unswtalk_button">
<a href='?register=1'>Register</a>
</td></tr>
</table>
</form>
eof
	} else {
		# create session file
		open(FH, ">", "/tmp/$id") or die $!;
		print FH "$id";
		close FH;
	}

    my $student_to_show  = $students[$n % @students];
    my $details_filename = "$student_to_show/student.txt";
    open my $p, "$details_filename" or die "can not open $details_filename: $!";
#$details = join '', <$p>;

	my @nos = split('/',  $student_to_show);
	my $nowStuNo = $nos[-1];

	my %stuhash;

    # read student information
	while (<$p>) {
		chomp;
		my $line = $_;

		my ($key, $value) = split(':', $line);

		$key = trim($key);
		$value = trim($value);

		$stuhash{lc($key)} = $value;
	}

    close $p;

    $details = "<table><tr><td valign='top'>";

	$table1 = "<table style='border:1px solid #ccc;'>";

	$table1 = $table1 . "<tr><td colspan=2>Welcome, <a href='?userid=$id'>$id</a>!&nbsp<a href='?userid=$id&logout=1'>Logout</a></td></tr>";

    # display the student information
	foreach my $k (keys %stuhash) {
		next if ($k eq "password");
		next if ($k eq "email");
		next if ($k eq "home_longitude");
		next if ($k eq "home_latitude");
		next if ($k eq "courses");
		next if ($k eq "friends");

		$row = "<tr><td>";
		$row = $row . $k;
		$row = $row . "</td><td>";
		$row = $row . $stuhash{$k};
		$row = $row . "</td></tr>";

		$table1 = $table1 . $row;
	}

    # display all friends
	my $unfri = param('unfriend') || '';
	if ($unfri ne '') {
		add_friend($id, $unfri, 0);
	}

	if (exists $stuhash{'friends'}) {
		$table1 = $table1 . "<tr><td colspan=2>";
		if ($stuhash{'friends'} =~ /\((.*)\)/) {
			@fri = split(',', $1);
			foreach my $f (@fri) {
				$f = trim($f);
				my $link = 0;
				foreach my $stu (@students) {
					my @stuno = split('/', $stu);
					if ($stuno[1] eq $f) {
						last;
					}
					$link += 1;
				}
                # display friends link
				my $href = "<p><a href='?n=$link&userid=$id'>";
				my $friendfile = "$students_dir/$f/student.txt";
				my $fname = getName($friendfile);
				$href = $href . $fname ."<br>";

				if (-e "$students_dir/$f/img.jpg") {
					$href = $href . "<img src=$students_dir/$f/img.jpg width=80 height=80 />";
				}
				$href = $href . "</a></p>";

				if ($id eq $nowStuNo) {
					$href .= "<a href='?userid=$id&unfriend=$f'>Unfriend</a>";
				}

				$table1 = $table1 . $href;
			}
		}

		$table1 = $table1 . "</td></tr>";
	}

	$table1 = $table1 . "</table>";

	$details = $details . $table1;

	$details = $details . "</td><td valign='top'>";

    # display all posts
	$table2 = "<table>Posts:";

	my $msgkey = param('msg_key') || '\\s+';

	$table2 = $table2 . "<form method='POST' action=''><tr><td colspan=4>Message:<input type=textbox name=msg_key style='width:400px;' value=''></input>&nbsp<input type=submit value=Search></input><input type=hidden name=userid value=$id></td></tr></form>";


	#posts 
	my %posthash;

    my @posts = sort (glob("$student_to_show/*.txt"));

	my $maxno = 0;

    # all posts by me and friends
	foreach my $post (@posts) {

		my @filename = split('/', $post);
		my $no = $filename[-1];
		$no =~ s/\.txt//;

		my @names = split('-', $no);
		next if (@names > 1);
		next if ($names[0] !~ /\d+/);

		if ($no > $maxno) {
			$maxno = $no;
		}

		my $row = "<tr><td>";
		$row = $row . $names[0];
		open my $po, $post or die "open post failed.";
		my %phash;
		my $timeline;
        # read posts
		while (<$po>) {
			chomp;

			my $line = $_;

			my ($key, $value) = split(':', $line);

			$key = trim($key);
			$value = trim($value);

			if (lc($key) eq "time") {
				$timeline = $value;
			}

			$phash{lc($key)} = $value;
			$phash{"no"} = $no;
		}

		$posthash{$timeline} = \%phash;

		close $po;
		$row = $row . "</td></tr>";

		#$table2 = $table2 . $row;
	}

	my $page = param('page') || 0;
	my $idx = 0;

	# display posts by time
	foreach my $pk (sort {$b cmp $a } keys %posthash) {
		my $row = "";
		if (($posthash{$pk}{"message"} =~ /$msgkey/) ) {

			if (($idx>=$page*16 && $idx <($page+1)*16)) {
				$row = "<tr><td>No.$idx, Author:</td><td>";
				$row = $row . $posthash{$pk}{"from"};
				$row = $row . "</td><td>Time:</td><td>$pk</td></tr>";
				$row = $row . "<tr><td>Content:</td><td colspan=3>";
				$row = $row . $posthash{$pk}{"message"};
				$row = $row . "<a href='?userid=$id'>Comment</a></td></tr>";
			}

			$idx += 1;
		}

		($row, $idx) = addPostRecursively(0, $posthash{$pk}{"no"}, \@posts, $row, $msgkey, $id, $page, $idx);

		if ($posthash{$pk}{"message"} =~ /$msgkey/) {
			if (($idx>=$page*16 && $idx <($page+1)*16))
			{
				$row = $row . "<tr><td>.</td></tr>";
			}
		}

		$table2 = $table2 . $row;
	}

    # page handler
	my $prev = $page - 1;
	if ($prev < 0) { $prev = 0;}

	my $next = $page + 1;

	$table2 .= "<tr><td colspan=4 align=right><a href='?userid=$id&page=$prev&msg_key=$msgkey'>Prev</a>&nbsp<a href='?userid=$id&page=$next&msg_key=$msgkey'>Next</a></td></tr>";

	$table2 = $table2 . "</table>";

	#opendir(DIR, $student_to_show) or die "opendir failed.";
	#while (defined($file = readdir(DIR))) {
	#	open my $postfd "$student_to_show/$file" or die "open post failed";
	#	while <$postfd> {
	#	}
	#}
	#closedir(DIR);

	$details = $details . $table2;

    # new post maker
	$table2_1 = "<form method='POST' action=''><table><tr><td colspan=2></td></tr><tr><td>Message Content:</td><td><input type=textbox name=content value='' style='width:400px' /><input type=hidden name=userid value=$id></td></tr><tr><td colspan=2><input type=submit value='Post New Message'></input></td></tr></table></form>";

	if ($student_to_show =~ /\/$id/) {
		$details .= $table2_1;
	}

	#add new post
	my $content = param('content') || '';
	if ($content ne '') {
		post_message($content, $id, $maxno+1);
	}

	$details = $details . "</td><td valign=top>";

	# friends search
	my $table3 = "<table>";
	my $search_stu = param('stu_name') || '';

	$table3 = $table3 . "<form method='POST' action=''><tr><td>StudentName:</td><td><input type=textbox name=stu_name value='$search_stu' /></td><td><input type=submit value=Search /><input type=hidden name=userid value=$id></td></tr></form>";
    
	my $uidx = 0;

	my $addfriend = param('addfriend') || '';
	if ($addfriend ne '') {
		add_friend($id, $addfriend, 1);
	}

	my $page1 = param('page1') || 0;

    # search result display
	if ($search_stu ne '') {
		my $link = 0;
		foreach my $onestu (@students) {
			my $name = getName("$onestu/student.txt");
			my @items = split('/', $onestu);
			my $frid = $items[-1];
			if ($name =~ /$search_stu/) {
				if ($uidx >= $page1*8 && $uidx < ($page1+1)*8) {
					$table3 = $table3 . "<tr><td colspan=2><a href='?userid=$id&addfriend=$frid'>Add</a>&nbsp&nbsp&nbsp&nbsp&nbsp<a href='?n=$link&userid=$id'>$name</td>";
					if (-e "$onestu/img.jpg") {
						$table3 = $table3 . "<td><img src=$onestu/img.jpg width=80 height=80 /></td>";
					} else {
						$table3 = $table3 . "<td></td>";
					}
					$table3 = $table3 . "</a></td></tr>";
				}

				$uidx += 1;
			}
			$link += 1;
		}
	}

    # page handler
	my $pv1 = $page1 - 1;
	if ($pv1 < 0) { $pv1 = 0; }
	my $pn1 = $page1 + 1;

	$table3 = $table3 . "<tr><td colspan=3 align=right><a href='?userid=$id&page1=$pv1&stu_name=$search_stu' >Prev</a>&nbsp<a href='?userid=$id&page1=$pn1&stu_name=$search_stu'>Next</a></td></tr></table>";

	$details = $details . $table3;

	$details = $details . "</td></tr></table>";

    my $next_student = $n + 1;
    return <<eof;
<div class="unswtalk_user_details">
$details
</div>
<p>
<form method="POST" action="">
    <input type="hidden" name="n" value="$next_student">
    <input type="hidden" name="userid" value="$id">
    <input type="submit" value="Next student" class="unswtalk_button">
</form>
eof
}


#
# HTML placed at the top of every page
#
sub page_header {
    return <<eof;
Content-Type: text/html; charset=utf-8

<!DOCTYPE html>
<html lang="en">
<head>
<title>unswtalk</title>
<link href="UNSWtalk.css" rel="stylesheet">
</head>
<body>
<div class="unswtalk_heading">
UNSWtalk 
<hr>
</div>
eof
}


#
# HTML placed at the bottom of every page
# It includes all supplied parameter values as a HTML comment
#
sub page_trailer {
    my $html = "<!--\n";
    foreach $param (param()) {
        my $value = param($param);
        $value =~ s/\>/\&gt;/g;
        $html .= "    $param='$value'\n"
    }
	$html .= "--";
	$html .= param('msg_key');
	$html .= "--";
    $html .= "-->\n";
    $html .= "</body>\n";
    $html .= "</html>\n";
    return $html;
}

main();

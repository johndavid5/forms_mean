use strict;

# Get rid of special color character sequences 
# such as...note that Ctrl-[ is \033
#
# [34m[2015-06-08 12:38:18.523] [TRACE] [default] - [39mEntering cheese testing
# [36m[2015-06-08 12:38:18.523] [DEBUG] [default] - [39mGot cheese.
# [32m[2015-06-08 12:38:18.524] [INFO] [default] - [39mCheese is Gouda.
# [32m[2015-06-08 12:38:18.524] [INFO] [default] - [39m
# [33m[2015-06-08 12:38:18.524] [WARN] [default] - [39mCheese is quite smelly.
# [31m[2015-06-08 12:38:18.524] [ERROR] [default] - [39mCheese gouda is too ripe!
# [35m[2015-06-08 12:38:18.524] [FATAL] [default] - [39mCheese was breeding ground for listeria.
# [36m[2015-06-08 12:38:18.524] [DEBUG] another - [39mJust checking
# [36m[2015-06-08 12:38:18.524] [DEBUG] pants - [39mSomething for pants
#
# It passes input unchanged to STDOUT
# but filtered to $sTeeFile...

$| = 1; # Turn on auto-flush for STDOUT...

my $sTeeFile = "";
my $bAppend = 0;

for( my $i = 0; $i < scalar(@ARGV); $i++ ){
	if( $ARGV[$i] =~ /-tee/i ){
		$sTeeFile = $ARGV[++$i];
	}

	if( $ARGV[$i] =~ /-a/i ){
		$bAppend = 1;
	}
}

my $OFH; # Output File Handle
my $sMode;

if( $bAppend ){
	$sMode = ">>";# append	
}
else {
	$sMode = ">"; # clobber
}

open($OFH, $sMode, $sTeeFile) 
  or die "Cannot open $sMode $sTeeFile: \"$!\"";

binmode($OFH);

# Turn on auto-flush for output file handle...
my $old_fh = select($OFH);
$| = 1;
select($old_fh);

my $line;

while($line = <STDIN>){

	print $line; # Print unchanged to STDOUT...

	# For log4js's output... 
	# e.g., "[36m[2015-06-08 12:38:18.524] [DEBUG] pants - [39mSomething for pants
	$line =~ s/\033\[3[0-9]m//g;

	# And now for karma's output...
	# e.g., "[1A[2KChrome 43.0.2357 (Windows 7 0.0.0): Executed 0 of 8 SUCCESS (0 secs / 0 secs)"
	$line =~ s/\033\[[0-9][A-Z]//g;

	print $OFH $line; # Print filtered to $sTeeFile...
}

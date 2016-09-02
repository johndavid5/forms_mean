use strict;

use constant WITHOUT=>"WITHOUT";
use constant WITHIN=>"WITHIN";

my $line_number = 0;
my $state = WITHOUT;
my $line = "";
my $block = "";
my $num_substitutions = 0;

while($line = <STDIN>){

	$line_number++;	
	print STDERR ("line[$line_number] = \"" . $line . "\"");

	if( $state eq WITHOUT ){
		if($line =~ /^(.*)JDA::Logger::log(.*)endl;(.*)$/ ){
			# A one-liner...
			#$num_substitutions = ($line =~ s/^(\s*)JDA::Logger::log(.*?)endl;\s*$/$1if( m_p_logger ){\n$1$1(*m_p_logger)$2endl;\n$1}/gms);
			$num_substitutions = fix_it( \$line );
			print STDERR ("/* num_substitutions = " . $num_substitutions . " */\n");
			print($line);
		}
		elsif($line =~ /^(.*)JDA::Logger::log/ ){
			$state = WITHIN;
			$block = $line;
		}
		else {
			print($line);
		}
	}
	elsif( $state eq WITHIN ){
		if($line =~ /endl;/ ){
			$block .= $line;
			#$num_substitutions = ($block =~ s/^(\s*)JDA::Logger::log(.*?)endl;\s*$/$1if( m_p_logger ){\n$1$1(*m_p_logger)$2endl;\n$1}/gms);
			$num_substitutions = fix_it( \$block );
			print STDERR ("/* num_substitutions = " . $num_substitutions . " */\n");
			print($block);
			$state = WITHOUT;
		}
		else{
			$block .= $line;
		}
	}

}# while($line = <STDIN>)

sub fix_it {
	my ($r_input) = @_;

	my $num_substitutions = (${$r_input} =~ s/^(\s*)JDA::Logger::log(.*?)endl;\s*$/$1if( m_p_logger ){\n$1$1(*m_p_logger)$2endl;\n$1}/gms);

	return $num_substitutions; 
}

#
#			if( m_p_logger ){
#				(*m_p_logger)
#			}
#

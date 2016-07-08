use strict;

my $input = <<EOF;
	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): SHEMP: Hey, Moe...JDA::WaldoUtils::b_perform_tactical_denormalizations is FALSE, so NOT callin' db_denormalize_subject_company() and db_denormalize_filing_agent(), Moe..." << endl;

	JDA::Logger::log(JDA::Logger::INFO) << sWho << "(): SHEMP: Hey, Moe...JDA::WaldoUtils::b_perform_tactical_denormalizations is FALSE,\n" 
	<< " so NOT callin' db_denormalize_subject_company() and db_denormalize_filing_agent(), Moe..." << endl;
EOF

print("BEFORE: input =\n" .
"-------------------\n" .
$input . "\n" .
"-------------------\n"
);
my $num_substitutions = $input =~ s/^(\s*)JDA::Logger::log((^endl)*)endl;\s*$/if( m_p_logger ){\n$1(*m_p_logger)$2endl;\n}/gms;
print("num_substitutions = $num_substitutions...\n");

print("AFTER: input =\n" .
"-------------------\n" .
$input . "\n" .
"-------------------\n"
);


#
#			if( m_p_logger ){
#				(*m_p_logger)
#			}
#

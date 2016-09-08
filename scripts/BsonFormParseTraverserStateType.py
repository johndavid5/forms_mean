# Python script to generate stateTypeToString() method body
# switch statement based on StateType enum...specifically 
# used for BsonFormParseTraverser.h...will detect when entering
# enum by seeing...
#    enum StateType {
# and detect end of enum by seeing...
#    };
#
# example input:
# 
#		enum StateType {
#
#			STATE_WITHOUT,
#				...
#				...
#				...
#							STATE_IN_ISSUER_MAIL_ADDRESS_DOC
#		};
#
#
# example output:
#
#		static string stateTypeToString( StateType stateType ){
#			switch( stateType ){	
#				case STATE_WITHOUT:
#					return "STATE_WITHOUT";
#				...
#				...	
#				...	
#				case STATE_IN_ISSUER_MAIL_ADDRESS_DOC:
#					return "STATE_IN_ISSUER_MAIL_ADDRESS_DOC";
#				default:
#					return "???";
#			}
#		}/* stateTypeToString() */
#

import sys
import re


### main() - begin ###

i_count = 0

patternStateTypeEnumStart = re.compile("enum\\s+StateType\\s+{") 
patternStateTypeEnumEnd= re.compile("};")

patternStateTypeTag = re.compile("STATE_(\w+)") 

STATE_WITHOUT = "STATE_WITHOUT"
STATE_IN_ENUM = "STATE_IN_ENUM"

state = STATE_WITHOUT

print "		static string stateTypeToString( StateType stateType ){" 
print "			switch( stateType ){"	


for line in sys.stdin:

	i_count += 1
	#print("line #%d: \"%s\"..." % ( i_count, line ) )

	if state == STATE_WITHOUT:
		list = patternStateTypeEnumStart.findall( line )

		if len(list) > 0 :
			#print ('Match on StateTypeEnumStart found: ', list[0])
			state = STATE_IN_ENUM
		
	elif state == "STATE_IN_ENUM":

		list = patternStateTypeEnumEnd.findall( line )

		if len(list) > 0 :
			#print ('Match on StateTypeEnumEnd found: ', list[0])
			#print ('SHEMP: Breaking out of loop, Moe...')
			break

		list = patternStateTypeTag.findall( line )

		if len(list) > 0 :
			#print ('Match on StateTypeTag found: ', list[0])
			tag = "STATE_" + list[0]
			#print ("tag = \"%s\"" % ( tag ) )

			print "				case " + tag + ":"
			print "					return \"" + tag + "\";"


print "				default:"
print "					return \"???\";"
print "			}/* switch( stateType ) */"
print "		}/* stateTypeToString() */"

print("Let off some %s, Bennett!" % ( "steam" ) )


### main() - end ###

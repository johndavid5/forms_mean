var jutils = require('../jutils');

var arrayWheres=
[
	[],
	[
		"date >= 2015-01-01"
	],
	[
		"date >= 2015-01-01",
		"name LIKE '%joe%'"
	]
];

for( var i = 0; i < arrayWheres.length; i++ ){
	console.log(
		"**********************\n" +
		"jutils.arrayWheresToStringWheres(\n" +
		JSON.stringify( arrayWheres[i] ) + ", \"\\t\", \"\\n\"\n" +
	") = \"" + jutils.arrayWheresToStringWheres( arrayWheres[i], "\t", "\n" ) + "\"...");
}


var arraySelects =
[
	[],
	[
		"name as stooge_name"
	],
	[
		"name as stooge_name",
		"stooge_id",
		"line as stooge_line"
	]
];

for( var i = 0; i < arraySelects.length; i++ ){
	console.log(
		"**********************\n" +
		"jutils.arraySelectsToStringSelects(\n" +
		JSON.stringify( arraySelects[i] ) + "\n" +
	") = \"" + jutils.arraySelectsToStringSelects( arraySelects[i] ) + "\"...");
}


var arrayJoins = 
[
	[],
	[
		"LEFT OUTER JOIN stooge_types st ON st.stooge_type = s.stooge_type"
	],
	[
		"LEFT OUTER JOIN stooge_types st ON st.stooge_type = s.stooge_type",
		"LEFT OUTER JOIN stooge_color sc ON sc.color_id = s.color_id"
	]
];

for( var i = 0; i < arrayJoins.length; i++ ){
	console.log(
		"**********************\n" +
		"jutils.arrayJoinsToStringJoins(\n" +
		JSON.stringify( arrayJoins[i] ) + "\n" +
	") = \"" + jutils.arrayJoinsToStringJoins( arrayJoins[i] ) + "\"...");
}


var myUndef;

var arrayStringToBools = [
	myUndef,
	null,
	0,
	1,
	true,
	false,
	"",
	"0",
	"1",
	"true",
	"TrUE",
	"false",
	"FaLsE",
	{},
	{"name": 'Moe'},
	[],
	[1,2,3]
];


console.log( "arrayStringToBools.length = ", arrayStringToBools.length, "...");

for( var i = 0; i < arrayStringToBools.length; i++ ){
	console.log(
		"**********************\n" +
		"arrayStringToBools[" + i + "] = \"" + arrayStringToBools[i] + "\" = <", arrayStringToBools[i], ">..."
	); 

	console.log(
		"[" + i + "]: jutils.stringToBool(", arrayStringToBools[i], ") = ", jutils.stringToBool( arrayStringToBools[i] ), "..."
	); 
}


var iAsInputs = [ "e.entities_id as ent_id",
	"e.entities_id",
	"entities_id"
	];

for( var i = 0; i < iAsInputs.length; i++ ){
	console.log(
		"**********************\n" +
		i + ": asIt( \"" + iAsInputs[i] + "\" ) = \"" + jutils.asIt( iAsInputs[i] ) + "\"...\n"
	); 
}

var s_params = [
	{ "sPrefix" : "", "options" : {} },	
	{ "sPrefix" : "", "options" : { "as_em" : true } },	
];

for( var i = 0; i < s_params.length; i++ ){
	var sPrefix = s_params[i].sPrefix;
	var options = s_params[i].options;

	console.log(
	"**********************\n" +
	i + ": arraySelectsToStringSelects( " + JSON.stringify(iAsInputs[i]) + " , \"" + sPrefix + "\", " + JSON.stringify(options) + " ) = \"" + jutils.arraySelectsToStringSelects( iAsInputs, sPrefix, options ) + "\"...\n"
); 
}

	var date = new Date();

	console.log(
	"**********************\n" +
	"jutils.dateTimeCompact(date) = \"" + jutils.dateTimeCompact(date) + "\"...\n" +
	"jutils.dateTimePretty(date) = \"" + jutils.dateTimePretty(date) + "\"...\n" +
	"date.toLocaleString() = \"" + date.toLocaleString() + "\"...\n"
	);

/*
	console.log("ORDINAL SUFFIXES:\n");	
	console.log("=================\n");

	for( var i = 0; i <= 30; i++ ){
		console.log( i + jutils.getOrdinalSuffix(i) + "...");
	}

	var ord_inputs = [ 100, 101, 102, 103, 104, 1000, 1001, 1002, 1003, 1004 ];

	for( var i = 0; i < ord_inputs.length; i++ ){
		console.log( ord_inputs[i] + jutils.getOrdinalSuffix(ord_inputs[i]) + "...");
	}
*/




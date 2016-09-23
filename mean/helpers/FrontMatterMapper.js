var jutils = require('../jutils');
var sharedUtils = require('../ng/shared-utils.svc.js');

var FrontMatterMapper = {

	frontMatterMap: [ 
				//{"field": "entities_id_filter", "field_pretty": "Entities ID" },
				{"field": "accession_number_filter", "field_pretty": "Accession Number" },
				{"field": "form_type_filter", "field_pretty": "Form Type" },
				{"field": "date_filed_from_filter", "field_pretty": "From Date" },
				{"field": "date_filed_to_filter", "field_pretty": "To Date" },
				{"field": "filing_agent_entity_name_varchar_filter", "field_pretty": "Filing Agent" },
				{"field": "filing_agent_cik_bigint_filter", "field_pretty": "Filing Agent CIK" },
				{"field": "subject_company_entity_name_varchar_filter", "field_pretty": "Subject Company" },
				{"field": "subject_company_cik_bigint_filter", "field_pretty": "Subject Company CIK" },
				{"field": "filer_name_filter", "field_pretty": "Filer Name" },
				{"field": "filer_cik_filter", "field_pretty": "Filer CIK" },
				{"field": "self_filed_filter", "field_pretty": "Self Filed" },
				{"field": "entity_cik_filter", "field_pretty": "Entity CIK" },
				{"field": "entity_name_filter", "field_pretty": "Entity Name" },
				{"field": "entity_type_filter", "field_pretty": "Entity Type" },
				{"field": "company_name_filter", "field_pretty": "Company Name" },
				{"field": "company_id_filter", "field_pretty": "Company ID" },
				{"field": "orderBy", "field_pretty": "Order By",
					"field_transform": function(field, query){
						field += " " + query.ascDesc;
						field = field.replace("_", " ");
						field = sharedUtils.ucFirstAllWords(field);
						return field;
					}
				},
				{"field": "email_address_filter", "field_pretty": "Person Email Address" },
				{"field": "first_name_filter", "field_pretty": "Person First Name" },
				{"field": "last_name_filter", "field_pretty": "Person Last Name" },
	],


	mapItUp: function( req_query ){

			// Add "Created: <date>"...no extra charge...
			req_query.frontMatter.push( "Created: " + jutils.dateTimePretty() );

			for( var i = 0; i < FrontMatterMapper.frontMatterMap.length; i++ ){
				var field_key = FrontMatterMapper.frontMatterMap[i].field;
				var field_pretty = FrontMatterMapper.frontMatterMap[i].field_pretty;
				var field_transform = FrontMatterMapper.frontMatterMap[i].field_transform;
				var field_value = req_query[ field_key ];
				if( field_value ){
					if( field_transform ){ 
						req_query.frontMatter.push( field_pretty + ": " + field_transform( field_value, req_query ) );
					}
					else {
						req_query.frontMatter.push( field_pretty + ": " + field_value );
					}
				}
			}

	}, /* mapItUp() */

}; /* var FrontMatterMapper */

module.exports = FrontMatterMapper;

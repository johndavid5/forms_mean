var db = require('../db');

var FormsIndex = db.model('forms_index',
	{	
		index_file: { type: String, required: true},
		company_name: {type: String, required: true},
		form_type: {type: String, required: true},
		date_filed: {type: String, required: true},
		file_name: {type: String, required: true}
	}
);

module.exports = FormsIndex;

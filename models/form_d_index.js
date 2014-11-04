var db = require('../db');

var FormDIndex = db.model('form_d_index',
	{	
		company_name: {type: String, required: true},
		form_type: {type: String, required: true},
		date_filed: {type: String, required: true},
		file_name: {type: String, required: true}
	}
);

module.exports = FormDIndex;

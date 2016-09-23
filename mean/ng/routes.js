angular.module('waldoApp').config( function($routeProvider){
	$routeProvider
	.when('/forms',
		{
			controller: 'FormsCtrl',
			templateUrl: 'forms.html',
			activeTab: 'forms'
		}
	)
	.when('/forms/:accessionNumber',
		{
			controller: 'FormsDetailCtrl',
			templateUrl: 'forms-detail.html',
			activeTab: 'forms'
		}
	)
	.when('/filings',
		{
			controller: 'FilingsCtrl',
			templateUrl: 'filings.html',
			activeTab: 'filings'
		}
	)
	.when('/filings-alpha',
		{
			controller: 'FilingsAlphaCtrl',
			templateUrl: 'filings-alpha.html',
			activeTab: 'filings'
		}
	)
	.when('/filings/:filingsId',
		{
			controller: 'FilingsDetailCtrl',
			templateUrl: 'filings-detail.html',
			activeTab: 'filings'
		}
	)
	.when('/entities',
		{
			controller: 'EntitiesCtrl',
			templateUrl: 'entities.html',
			activeTab: 'entities'
		}
	)
	.when('/entities/:entitiesId',
		{
			controller: 'EntitiesDetailCtrl',
			templateUrl: 'entities-detail.html',
			activeTab: 'entities'
		}
	)
	.when('/people',
		{
			controller: 'PeopleCtrl',
			templateUrl: 'people.html',
			activeTab: 'people'
		}
	)
	.when('/people-alpha',
		{
			controller: 'PeopleAlphaCtrl',
			templateUrl: 'people-alpha.html',
			activeTab: 'people'
		}
	)
	.when('/people/:peopleId',
		{
			controller: 'PeopleDetailCtrl as peopleDetailCtrl',
			templateUrl: 'people-detail.html',
			activeTab: 'people'
		}
	)
	.when('/companies',
		{
			controller: 'CompaniesCtrl',
			templateUrl: 'companies.html',
			activeTab: 'companies'
		}
	)
	.when('/companies/:companiesId',
		{
			controller: 'CompaniesDetailCtrl',
			templateUrl: 'companies-detail.html',
			activeTab: 'companies'
		}
	)
	.when('/search',
		{
			controller: 'SearchCtrl',
			templateUrl: 'search.html',
			activeTab: 'search'
		}
	)
	.when('/register',
		{
			controller: 'RegisterCtrl',
			templateUrl: 'register.html',
			activeTab: 'register'
		}
	)
	.when('/login',
		{
			controller: 'LoginCtrl',
			templateUrl: 'login.html',
			activeTab: 'login'
		}
	)
	.when('/utils/isql',
		{
			controller: 'IsqlCtrl',
			templateUrl: 'isql.html',
			activeTab: 'utils_isql'
		}
	)
	.when('/*',
		{
			redirectTo: '/forms'
		}
	)
	.otherwise(
		{
			redirectTo: '/forms'
		}
	);
});

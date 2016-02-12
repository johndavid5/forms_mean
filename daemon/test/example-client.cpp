/* gcc example.c -o example $(pkg-config --cflags --libs libmongoc-1.0) */

/* ./example-client [CONNECTION_STRING [COLLECTION_NAME]] */

#include <mongoc.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>
using namespace std;

int
main (int   argc,
      char *argv[])
{
   /* Get C and C++ I/O to play nice together... */
   cout.sync_with_stdio(true);

   mongoc_client_t *client;
   mongoc_collection_t *collection;
   mongoc_cursor_t *cursor;
   bson_error_t error;
   const bson_t *doc;

   const char *uri_str = "mongodb://127.0.0.1/";
   const char *db_name = "test";
   const char *collection_name = "test";
   string s_json = "";

   bson_t query;
   bson_t* p_bson_query;
   char *str;

   //if (argc > 1) {
   //   uri_str = argv [1];
   //}

   //if (argc > 2) {
   //   collection_name = argv [2];
   //}

   for(int i = 0 ; i < argc; i++ ){
     if( strcmp( argv[i], "-uri" ) == 0 && i+1 < argc ){ 
		uri_str = argv[++i];
	 }
     if( strcmp( argv[i], "-db" ) == 0 && i+1 < argc ){ 
		db_name = argv[++i];
	 }
     if( strcmp( argv[i], "-collection" ) == 0 && i+1 < argc ){ 
		collection_name = argv[++i];
	 }
     if( strcmp( argv[i], "-json" ) == 0 && i+1 < argc ){ 
		s_json = argv[++i];
	 }
   }

   cout << "uri_str = \"" << uri_str << "\"..." << endl;
   cout << "db_name = \"" << db_name << "\"..." << endl;
   cout << "collection_name = \"" << collection_name << "\"..." << endl;
   cout << "s_json = \"" << s_json.c_str() << "\"..." << endl;

   cout << "Calling mongoc_init()..." << endl;
   mongoc_init ();


   cout << "Calling mongoc_client_new( uri_str = \"" << uri_str << "\" )..." << endl;
   client = mongoc_client_new (uri_str);

   if (!client) {
      fprintf (stderr, "Failed to parse URI.\n");
      return EXIT_FAILURE;
   }

   bson_init (&query);

#if 0
   bson_append_utf8 (&query, "hello", -1, "world", -1);
#endif
   
	if( s_json.length() > 0 ){
		cout << "Calling bson_new_from_json( \"" << s_json.c_str() << "\" )..." << endl;
		p_bson_query = bson_new_from_json( (const uint8_t *) s_json.c_str(), -1, &error );
		if( ! p_bson_query ){
			cerr << "Trouble converting json to bson: \"" << error.message << "\"" << endl;
			return EXIT_FAILURE;
		}
	}
	else {
		p_bson_query = &query;
	}

    str = bson_as_json (p_bson_query, NULL);
	cerr << "bson_as_json = \"" << str << "\"..." << endl;
	string s_bson_as_json = str;
    bson_free (str);

	cerr << "s_bson_as_json = \"" << s_bson_as_json.c_str() << "\"..." << endl;


   cout << "Getting db_name = \"" << db_name << "\", collection_name = \"" << collection_name << "\"..." << endl;
   //collection = mongoc_client_get_collection (client, "test", collection_name);
   collection = mongoc_client_get_collection (client, db_name, collection_name);

   cout << "Running db." << collection_name << ".find( " << s_bson_as_json << " )..." << endl;
   cout << endl;

   cursor = mongoc_collection_find (collection,
                                    MONGOC_QUERY_NONE,
                                    0,
                                    0,
                                    0,
                                    //&query,
									p_bson_query,
                                    NULL,  /* Fields, NULL for all. */
                                    NULL); /* Read Prefs, NULL for default */

   int index = -1;

   while (mongoc_cursor_next (cursor, &doc)) {
      index++;
      str = bson_as_json (doc, NULL);
      fprintf (stdout, "[%d] %s\n\n", index, str);
      bson_free (str);
   }

   if (mongoc_cursor_error (cursor, &error)) {
      fprintf (stderr, "Cursor Failure: %s\n", error.message);
      return EXIT_FAILURE;
   }

   //bson_destroy (&query);
   cout << "Calling bson_destroy( p_bson_query )..." << endl;
   bson_destroy(p_bson_query); 
   cout << "Terminated..." << endl;

   mongoc_cursor_destroy (cursor);
   mongoc_collection_destroy (collection);
   mongoc_client_destroy (client);

   mongoc_cleanup ();

   cout << "Let off some collections, Bennett!" << endl;

   cout << "Let off some steam, Bennett!" << endl;

   return EXIT_SUCCESS;
}

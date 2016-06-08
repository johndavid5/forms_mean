build = ./build
bin = ./build

#bson_inc_dir = /libbson/include
bson_inc_dir = /libbson/include/libbson-1.0

#bson_lib_dir = /libbson/lib
bson_lib_dir = ../../../../../../libbson/lib
bson_lib = /libbson/lib/bson-1.0.lib
bson_lib_static = /libbson/lib/bson-static-1.0.lib

#mongodb_inc_dir = /mongodb/include
mongodb_inc_dir = /mongodb/include/libmongoc-1.0

#mongodb_lib_dir = /mongodb/lib
mongodb_lib_dir = ../../../../../../mongodb/lib
mongodb_lib = /mongodb/lib/mongoc-1.0.lib
mongodb_lib_static = /mongodb/lib/mongoc-static-1.0.lib


CC=cl /EHsc /I. /I"$(mongodb_inc_dir)" /I"$(bson_inc_dir)" 

#CFLAGS = -c -DSTRICT -W3 -Zp -Tp 

CFLAGSMT = -c -DSTRICT -W3 -Zp -Tp 

GUILIBS = wsock32.lib advapi32.lib user32.lib ws2_32.lib Secur32.lib
#GUILIBS = wsock32.lib advapi32.lib user32.lib ws2_32.lib Secur32.lib libcmt.lib
GUILIBS = wsock32.lib advapi32.lib user32.lib ws2_32.lib Secur32.lib msvcrt.lib

# To Prevent "MSVCRT.lib(MSVCR100.dll) : error LNK2005: _calloc already defined in LIBCMT.lib(calloc.obj)" [caused by including curl libs...]
GUIFLAGS = /NODEFAULTLIB:libcmt.lib
GUIFLAGS = $(GUIFLAGS) /NODEFAULTLIB:"msvcrtd.lib"

LINKER=link

OBJS = $(build)/example-client.obj

$(bin)/example-client.exe : $(OBJS)
	$(LINKER) $(GUIFLAGS) -OUT:$(bin)/example-client.exe $(OBJS) $(GUILIBS) "$(mongodb_lib_dir)/mongoc-static-1.0.lib" "$(bson_lib_dir)/bson-static-1.0.lib"

$(build)/example-client.obj : ./example-client.cpp 
     $(CC) /Fo$(build)/example-client.obj $(CFLAGSMT) ./example-client.cpp

# -------------------------------------------------------------
#  MSC.BAT -- Set up environment for Microsoft C/C++ 7.0 NMAKE
# -------------------------------------------------------------
# /Zp[n] pack structs on n-byte boundary
# /TP compile all files as .cpp
# /MT link with LIBCMT.LIB

utlibdir =../../../gtest/msvc/build
utincdir =../../../gtest/include

#libdir =../../cpp_lib
libdir =..

#irrxmldir=../../cpp_lib/irrxml-1.2/src

#curl_inc_dir = ../../libcurl/include
#curl_lib_dir = ../../libcurl/builds/libcurl-vc-x86-release-static-ipv6-sspi-spnego-winssl-obj-lib

build=./build

#CC=cl /EHsc /I. /I.. /I$(libdir) /I$(utincdir) /I$(irrxmldir) /I"$(mysql_inc_dir)" /I$(curl_inc_dir)
CC=cl /EHsc /I. /I.. /I$(libdir) /I$(utincdir) 

#CFLAGS=-c -DSTRICT -D_CRT_SECURE_NO_WARNINGS -W3 -Zp -Tp 
CFLAGS = -c -DSTRICT -W3 -Zp -Tp 

#CFLAGSMT=-c -DSTRICT -MT -D_CRT_SECURE_NO_WARNINGS -W3 -Zp -Tp 
#CFLAGSMT = -c -DSTRICT -MT -W3 -Zp -Tp 
# MySQL's Flags below...
#CFLAGSMT=-c /MT /Zi /O2 /Ob1 /D NDEBUG -DDBUG_OF
#CFLAGSMT=-c /MT /Zi /O2 /Ob1
#CFLAGSMT = -c -DSTRICT -W3 -Zp -Tp 
# IMPORTANT: Use -DCURL_STATICLIB to pull in the CURL static libraries...
CFLAGSMT = -c -DSTRICT -DCURL_STATICLIB -W3 -Zp -Tp 

LINKER=link

# MySQL's GUILIBS...
GUILIBS = wsock32.lib advapi32.lib user32.lib ws2_32.lib Secur32.lib

# MSVCRT.lib(MSVCR100.dll) : error LNK2005: _calloc already defined in LIBCMT.lib(calloc.obj)
#GUIFLAGS = /NODEFAULTLIB:libcmt.lib

#OBJS = tests.obj IrrPassThroughFilterTest.cppunit.obj IrrPassThroughFilter.obj irrXML.obj utils.obj logger.obj 
#OBJS = $(build)/tests.obj $(build)/TestsCommon.obj $(build)/UtilsTest.obj $(build)/utils.obj $(build)/logger.obj 
OBJS = $(build)/tests.obj $(build)/TestsCommon.obj $(build)/UtilsTest.obj $(build)/utils.obj

$(build)/tests.exe : $(OBJS) $(libdir)/logger.h $(utlibdir)/gtest.lib 
	$(LINKER) $(GUIFLAGS) -OUT:$(build)/tests.exe $(OBJS) $(utlibdir)/gtest.lib $(GUILIBS)

$(utlibdir)/gtest.lib: 
	cd $(utlibdir)
	nmake /e /f gtest.mak
	cd ../../../forms_mean/daemon/cpp_lib/test

$(build)/tests.obj : tests.cpp
     $(CC) /Fo$(build)/tests.obj $(CFLAGSMT) tests.cpp

$(build)/TestsCommon.obj : TestsCommon.cpp
     $(CC) /Fo$(build)/TestsCommon.obj $(CFLAGSMT) TestsCommon.cpp

$(build)/UtilsTest.obj : ./UtilsTest.cpp 
     $(CC) /Fo$(build)/UtilsTest.obj $(CFLAGSMT)./UtilsTest.cpp

$(build)/utils.obj : $(libdir)/utils.cpp $(libdir)/utils.h 
    $(CC) /Fo$(build)/utils.obj $(CFLAGSMT) $(libdir)/utils.cpp

#$(build)/logger.obj : $(libdir)/logger.cpp $(libdir)/logger.h
#    $(CC) /Fo$(build)/logger.obj $(CFLAGSMT) $(libdir)/logger.cpp

clean:
	del "$(build)/*.obj" "$(build)/tests.exe"

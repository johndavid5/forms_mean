# -------------------------------------------------------------
#  MSC.BAT -- Set up environment for Microsoft C/C++ 7.0 NMAKE
# -------------------------------------------------------------
# /Zp[n] pack structs on n-byte boundary
# /TP compile all files as .cpp
# /MT link with LIBCMT.LIB

build = ./build
#bin = ../bin
bin = ./build

#cpp_lib_inc_dir = ../../cpp_lib
cpp_lib_inc_dir = ./cpp_lib

forms_mean_inc_dir = .

boost_inc_dir = ../../boost
boost_lib_dir = ../../boost/build/libs

curl_inc_dir = ../../libcurl/include
curl_lib_dir = ../../libcurl/builds/libcurl-vc-x86-release-static-ipv6-sspi-spnego-winssl-obj-lib

CC=cl /EHsc /I. /I"$(cpp_lib_inc_dir)" /I"$(forms_mean_inc_dir)" /I"$(boost_inc_dir)" /I"$(curl_inc_dir)"

#CFLAGS=-c -DSTRICT -D_CRT_SECURE_NO_WARNINGS -W3 -Zp -Tp 
#CFLAGS = -c -DSTRICT -W3 -Zp -Tp 

#CFLAGSMT=-c -DSTRICT -MT -D_CRT_SECURE_NO_WARNINGS -W3 -Zp -Tp 
#CFLAGSMT = -c -DSTRICT -MT -W3 -Zp -Tp 
# MySQL's Flags below...
#CFLAGSMT=-c /MT /Zi /O2 /Ob1 /D NDEBUG -DDBUG_OF
#CFLAGSMT=-c /MT /Zi /O2 /Ob1

# IMPORTANT: Use -DCURL_STATICLIB to pull in the CURL static libraries...
# FtpClient.obj : error LNK2019: unresolved external symbol __imp__curl_global_cleanup referenced in function ...
CFLAGSMT = -c -DSTRICT -DCURL_STATICLIB -W3 -Zp -Tp 

LINKER=link

#GUILIBS = 
# MySQL's GUILIBS...
GUILIBS = wsock32.lib advapi32.lib user32.lib ws2_32.lib Secur32.lib

# To Prevent "MSVCRT.lib(MSVCR100.dll) : error LNK2005: _calloc already defined in LIBCMT.lib(calloc.obj)" [caused by including curl libs...]
#GUIFLAGS = /NODEFAULTLIB:libcmt.lib

# Prevent boost lib from doing auto-link's...
GUIFLAGS = $(GUIFLAGS) /NODEFAULTLIB:"libboost_system-vc100-mt-1_55.lib"
GUIFLAGS = $(GUIFLAGS) /NODEFAULTLIB:"libboost_filesystem-vc100-mt-1_55.lib"
GUIFLAGS = $(GUIFLAGS) /NODEFAULTLIB:"libboost_system-vc100-mt-gd-1_55.lib"
GUIFLAGS = $(GUIFLAGS) /NODEFAULTLIB:"libboost_filesystem-vc100-mt-gd-1_55.lib"
GUIFLAGS = $(GUIFLAGS) /NODEFAULTLIB:"libboost_filesystem-vc100-mt-s-1_55.lib"
GUIFLAGS = $(GUIFLAGS) /NODEFAULTLIB:"libboost_system-vc100-mt-s-1_55.lib"

OBJS = $(build)/forms_mean_daemon.obj $(build)/FormsMeanUtils.obj $(build)/FormsMeanCommon.obj $(build)/utils.obj 

$(bin)/forms_mean_daemon.exe : $(OBJS)
	$(LINKER) $(GUIFLAGS) -OUT:$(bin)/forms_mean_daemon.exe $(OBJS) "$(curl_lib_dir)/libcurl_a.lib" $(GUILIBS) $(boost_lib_dir)/filesystem.lib $(boost_lib_dir)/system.lib

$(build)/forms_mean_daemon.obj : ./forms_mean_daemon.cpp 
     $(CC) /Fo$(build)/forms_mean_daemon.obj $(CFLAGSMT) ./forms_mean_daemon.cpp

$(build)/FormsMeanUtils.obj : $(forms_mean_inc_dir)/FormsMeanUtils.cpp $(forms_mean_inc_dir)/FormsMeanUtils.h
     $(CC) /Fo$(build)/FormsMeanUtils.obj $(CFLAGSMT) $(forms_mean_inc_dir)/FormsMeanUtils.cpp

$(build)/FormsMeanCommon.obj : $(forms_mean_inc_dir)/FormsMeanCommon.cpp $(forms_mean_inc_dir)/FormsMeanCommon.h
     $(CC) /Fo$(build)/FormsMeanCommon.obj $(CFLAGSMT) $(forms_mean_inc_dir)/FormsMeanCommon.cpp

$(build)/utils.obj : $(cpp_lib_inc_dir)/utils.cpp $(cpp_lib_inc_dir)/utils.h
     $(CC) /Fo$(build)/utils.obj $(CFLAGSMT) $(cpp_lib_inc_dir)/utils.cpp

clean:
	del "$(build)\*.obj" "$(build)\*.exe" 

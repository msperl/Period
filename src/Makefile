DISTRIBUTE_ADDITIONAL=../Period98Package
include ../../setup/general/makeapp

SHELL=/bin/sh

reversion::
	@(VERSION=`cat Version`;\
	 DATE=`date +'%Y-%m-%d-%H:%M'`;\
	 QUOTE='"';\
	 echo "const char *versiontxt=$${QUOTE}V$$VERSION$$QUOTE;" \
	 > version.cpp;\
	 echo "const char *versiondatetxt=$$QUOTE$$DATE$$QUOTE;" \
	 >> version.cpp;\
	)

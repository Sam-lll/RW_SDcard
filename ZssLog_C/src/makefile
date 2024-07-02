include ../MS_makefile_template

# EXES = dllmain.exe
OBJS = ZssLog.obj Util.obj

# SRC_FILE = ZssLog.cpp Util.cpp

all: $(OBJS)
	$(LINK) /DLL /OUT:ZssLog.dll $(OBJS)
	xcopy *.lib ..\lib\ /y
	xcopy *.dll ..\bin\ /y
	xcopy *.dll ..\dlltest\ /y
# ZssLog.obj: ZssLog.cpp
# 	$(CPP) $(CPPFLAGS) /c ZssLog.cpp

# Util.obj: Util.cpp
# 	$(CPP) $(CPPFLAGS) /c Util.cpp

.cpp.obj: 
	$(CPP) $(CPPFLAGS) /c $<

.cpp.exe: 
	$(LINK) $(OBJS)

clean:
	del *.obj *.dll *.exp *.lib

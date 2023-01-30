@echo off
cd ..\src
cl /EHsc /Fo"..\\build\\" /I "..\\include\\" /I "..\\include\\glad\\" /I "..\\include\\GLFW\\" /I "..\\include\\KHR\\" /c^
 game.cpp^
 asset.cpp^
 camera.cpp^
 cursor.cpp^
 entity.cpp^
 glad.c^
 light.cpp^
 mdcla.cpp^
 shader.cpp^
 sound.cpp
cd ..\build
link /NODEFAULTLIB:"msvcrtd.lib" /MACHINE:X64 /LIBPATH:"..\\libs\\"^
 game.obj^
 asset.obj^
 camera.obj^
 cursor.obj^
 entity.obj^
 glad.obj^
 light.obj^
 mdcla.obj^
 shader.obj^
 sound.obj^
 glfw3_mt.lib^
 gdi32.lib^
 user32.lib^
 kernel32.lib^
 shell32.lib^
 opengl32.lib^
 /OUT:firstGame.exe
cd ..\build
del *.obj
@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
cd F:\src
cl -nologo -MT -GR- -Oi -WX -W4 -wd4100 -Z7 -EHsc -Fo"..\\build\\" -I "..\\include\\" -I "..\\include\\glad\\" -I "..\\include\\GLFW\\" -I "..\\include\\KHR\\" -c^
 game.cpp^
 asset.cpp^
 camera.cpp^
 cursor.cpp^
 entity.cpp^
 glad.c^
 light.cpp^
 mdcla.cpp^
 shader.cpp^
 sound.cpp^
 asset_manager.cpp
cd ..\build
link -nologo -NODEFAULTLIB:"msvcrtd.lib" -MACHINE:X64 -DEBUG:FULL -LIBPATH:"..\\libs\\"^
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
 asset_manager.obj^
 glfw3_mt.lib^
 gdi32.lib^
 user32.lib^
 kernel32.lib^
 shell32.lib^
 opengl32.lib^
 /OUT:firstGame.exe
cd ..\build
del *.obj

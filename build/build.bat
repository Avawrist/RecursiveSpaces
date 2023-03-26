@echo off
if not defined DevEnvDir (
   call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
)
cd F:\src
cl -DASSERTIONS=1 -nologo -MT -GR- -Oi -WX -W4 -wd4100 -wd4189 -Z7 -EHsc -Fo"..\\build\\" -I "..\\include\\" -I "..\\include\\glad\\" -I "..\\include\\GLFW\\" -I "..\\include\\KHR\\" -c^
 game.cpp^
 asset.cpp^
 glad.c^
 mdcla.cpp^
 input.cpp^
 draw.cpp^
 ecs.cpp^
 platform.cpp
cd ..\build
link -nologo -NODEFAULTLIB:"msvcrtd.lib" -MACHINE:X64 -DEBUG:FULL -LIBPATH:"..\\libs\\"^
 game.obj^
 asset.obj^
 glad.obj^
 mdcla.obj^
 input.obj^
 draw.obj^
 ecs.obj^
 platform.obj^
 glfw3_mt.lib^
 gdi32.lib^
 user32.lib^
 kernel32.lib^
 shell32.lib^
 opengl32.lib^
 winmm.lib^
 /OUT:firstGame.exe
cd ..\build
del *.obj

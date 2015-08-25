## when fucntion is declare as __stdcall, then there are only one method to use its dll in mingw32 compiled under msvc
 
##method 1: use pexports to produce a .def file; dlltool to produce .a import library; link the .a
pexports teststdcall.dll | sed "s/^_//" > teststdcall.def
dlltool -U -d teststdcall.def -l libteststdcall.a
mv teststdcall.lib teststdcall.lib-bak
g++ -o stdcallapp-mingw.exe stdcallapp.cpp -L ../debug -l teststdcall
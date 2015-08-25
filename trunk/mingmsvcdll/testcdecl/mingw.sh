## when fucntion is declare as __cdecl, then there are two method to use its dll in mingw32 compiled under msvc
##method 1: directly link the export lib that produced by msvc when compile the dll
g++ -o cdeclapp-mingw.exe cdeclapp.cpp -L ../debug -l testcdecl
 
##method 2: use pexports to produce a .def file; dlltool to produce .a import library; link the .a
pexports testcdecl.dll > testcdecl.def
dlltool -d testcdecl.def -l libtestcdecl.a
mv testcdecl.lib testcdecl.lib-bak
g++ -o cdeclapp-mingw.exe cdeclapp.cpp -L ../debug -l testcdecl

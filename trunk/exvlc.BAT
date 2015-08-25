
del libvlc.def1 
del libvlc.def
del libvlc.exp 
del libvlc.lib

call "C:\Program Files\Microsoft Visual Studio 8\VC\vcvarsall.bat"

dumpbin /exports libvlc.dll > libvlc.def

echo EXPORTS > libvlc.def1

for /F "skip=19 tokens=4" %%i in (libvlc.def) do echo %%i >> libvlc.def1

del libvlc.def

rename libvlc.def1 libvlc.def

lib /def:"libvlc.def" /out:libvlc.lib /machine:x86

for %%i in (libavcodec_plugin.dll
libblend_plugin.dll
libdirect3d_plugin.dll
libdirectx_plugin.dll
libdrawable_plugin.dll
libfreetype_plugin.dll
libi420_rgb_sse2_plugin.dll
liblive555_plugin.dll
libpacketizer_h264_plugin.dll
libpng_plugin.dll
libqt4_plugin.dll
libscale_plugin.dll
libswscale_plugin.dll
libts_plugin.dll
libvout_wrapper_plugin.dll
libxml_plugin.dll)  do (
 xcopy plugins\%%i plugins-clip\ /q /y
)

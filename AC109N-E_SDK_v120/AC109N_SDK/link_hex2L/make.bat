copy ..\link_hex6L\info.txt .\ /y
cd ..\link_hex
del myoutotp.bin.fw
maker -c image.cfg
download.bat
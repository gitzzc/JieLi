%~d0
cd %~p0
Hex -tohex -s 0x0000 -l 0x8000 -b 8 -f 0xff -e .\debug\exe\app.hex .\link_hex2L\app.hex
del .\debug\exe\app.hex
cd link_hex2L
make.bat
cd..
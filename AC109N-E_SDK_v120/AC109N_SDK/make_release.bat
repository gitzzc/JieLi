%~d0
cd %~p0
Hex -tohex -s 0x0000 -l 0x4000 -b 8 -f 0xff -e release\exe\app.hex link_hex6L\app.hex
del release\exe\app.hex
cd link_hex6L
make.bat
cd..
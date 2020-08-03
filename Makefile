
all:
	$(CC) src/mitewm.c -o ./mitewm -lcairo -lX11
	
set_xinitrc:
	echo "xcompmgr & ${CURDIR}/mitewm & xterm" > ~/.xinitrc

clean:
	rm -f mitewm

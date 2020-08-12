
all:
	$(CC) ${CURDIR}/src/mitewm.c  -lcairo -lX11 -ljson-c -o ${CURDIR}/mitewm
	
set_xinitrc:
	echo "xcompmgr & ${CURDIR}/mitewm ${CURDIR}/mitewm_config.json & xterm" > ~/.xinitrc

create_config:
	echo "\n{\n\t\"background_image_file\" : \"\"\n}\n" > ${CURDIR}/mitewm_config.json

clean:
	rm -f mitewm


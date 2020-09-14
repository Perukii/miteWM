

XINITRC=~/.xinitrc

all:
	$(CC) ${CURDIR}/src/mitewm.c  -lcairo -lX11 -ljson-c -o ${CURDIR}/mitewm
	
set_xinitrc:
	echo "xcompmgr & ${CURDIR}/mitewm ${CURDIR}/mitewm_config.json & xterm" > $(XINITRC)

create_config:
	echo "\n{\n\
		\"background_image_file\" : \"${CURDIR}/resources/background_image_file/mitewm_back.png\"\
		\n}\n" > ${CURDIR}/mitewm_config.json

clean:
	rm -f mitewm


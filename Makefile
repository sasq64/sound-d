
all :
	make -C viceplugin/build
	make -C hivelyplugin/build
	make -C openmptplugin/build
	dub


find . -type d | xargs chmod 755
fakeroot dpkg-deb --build debian
lintian debian.deb

Estructura de directorios:
debian:
DEBIAN
usr

debian/DEBIAN:
control

debian/usr:
bin
lib

debian/usr/bin:
djon

debian/usr/lib:
libdjon-util.so
libdjon-util.so.1
libdjon-util.so.1.0
libdjon-util.so.1.0.0

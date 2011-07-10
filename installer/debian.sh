#!/bin/sh


## find packages

dkpg -S 
#updates the current binary files
cp ../libs/djon debian/usr/bin/
cp ../libs/libdjon-util.so ../libs/libdjon-util.so.1 ../libs/libdjon-util.so.1.0 ../libs/libdjon-util.so.1.0.0 debian/usr/lib/

# dpkg -S /bin/cat
# apt-cache showpkg libxss1
find debian -type d | xargs chmod 755
fakeroot dpkg-deb --build debian djon.deb
lintian djon.deb


ldd ./djon

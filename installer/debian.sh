fakeroot dpkg-deb --build debian
lintian debian.deb
dpkg-deb --build debian
find . -type d | xargs chmod 755

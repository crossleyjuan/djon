mkdir djon.app/Contents/Frameworks
cp libdjon-util.1.dylib djon.app/Contents/Frameworks/
install_name_tool -id @executable_path/../Frameworks/libdjon-util.1.dylib djon.app/Contents/Frameworks/libdjon-util.1.dylib
install_name_tool -change libdjon-util.1.dylib @executable_path/../Frameworks/libdjon-util.1.dylib djon.app/Contents/MacOS/djon
macdeployqt djon.app -dmg

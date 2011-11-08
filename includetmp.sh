#!/bin/sh

DIRLIST="djon-util djon-data djon-plugins djon"

TEMPLATE="template.txt"

for FILE in $(find . -iwholename '*/*.cpp'); do
	if test -z $(grep -q "This file is part of " $FILE);
	then
             DESTFILE="$FILE.tmp";
             cat $TEMPLATE $FILE > $DESTFILE;
	fi
done


 for FILE in $(find . -iwholename '*/*.cpp'); do
      TMPFILE="$FILE.tmp";
      echo "moving $TMPFILE to $FILE"
      mv $TMPFILE $FILE
 done


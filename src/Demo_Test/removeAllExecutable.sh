#!/bin/bash
# since Bash v4


# Remove all the executable_x in the directory.
for fichier in *.cpp; do 
baseNameFile=$(basename $fichier .${fichier##*.}); 
extension='_x'
NameFileExtension=$baseNameFile$extension
rm -v $NameFileExtension
done

#!/bin/bash
# since Bash v4

# Compile all the cpp file in the directory to create executable_x.
for fichier in *.cpp; do 
baseNameFile=$(basename $fichier .${fichier##*.}); 
extension='_x'
NameFileExtension=$baseNameFile$extension
echo $NameFileExtension
make $NameFileExtension
done

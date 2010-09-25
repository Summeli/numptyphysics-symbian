#!/bin/sh -x
# simple script to prepare numptywin32 for release

# folder in which we create the binary distro
DIST=NumptyPhysicsWin32

# shared libraries to be put into the dist
DLLS="SDL.dll SDL_image.dll jpeg.dll libpng12-0.dll zlib1.dll"

# level files and data files
DATA="*.jpg *.png *.nph"

# misc files to be placed into the distro
MISC="README.win32"

# binary executable, when compiled
BINARY=Game.exe

# target name in distro folder (without .exe)
NAME=NumptyPhysics

make
rm -rf $DIST
mkdir $DIST
mkdir $DIST/data
cp i686/$BINARY $DIST/$NAME.exe
cp $DATA $DIST/data
cp $DLLS $DIST
cp $MISC $DIST
find $DIST


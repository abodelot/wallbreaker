#!/bin/sh
# Stand-alone release builder for Linux 

# Go to root directory of the git repository
ROOT_PATH=$(git rev-parse --show-cdup)
if [ $ROOT_PATH ]; then
	cd $ROOT_PATH
fi

# Get project name from directory
PROJECT_NAME=$(basename $(pwd) | tr '[A-Z]' '[a-z'])

# Ask which version to build (will be included in filename)
read -p "Enter version number: " VERSION

# Name of the directory which will contain the release
ARCH=$(uname -m)
KERNEL=$(uname -s | tr '[A-Z]' '[a-z]')
TARGET_NAME="$PROJECT_NAME"_"$VERSION"-"$KERNEL"_"$ARCH"
echo Packaging $TARGET_NAME.tar.gz ...

# Find SFML libraries
for i in /usr/lib /usr/local/lib; do
	SFML_LIBS=$i/libsfml*
	files=$(ls $SFML_LIBS* 2> /dev/null | wc -l)
	if [ $files != "0" ]; then
		break
	fi
done

# Copy SFML libraries
mkdir bin/lib
cp $SFML_LIBS bin/lib

# Copy launcher script
cp tools/wallbreaker-launcher.sh bin

# Copy README
cp README.md bin

# Create the tarball from bin directory and give it a proper name
tar -czf $TARGET_NAME.tar.gz bin/ --transform s/bin/$TARGET_NAME/ 

# Clean up the mess created in the bin/ directory
rm -r bin/lib
rm bin/wallbreaker-launcher.sh
rm bin/README.md

echo Done!


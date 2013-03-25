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
TARGET_NAME="$PROJECT_NAME"_"$VERSION"-linux
echo Packaging $TARGET_NAME.tar.gz ...

# Copy SFML libraries
mkdir bin/lib32
cp /usr/local/lib/libsfml* bin/lib32

# Copy launcher script
cp tools/wallbreaker-launcher.sh bin

# Copy README
cp README.md bin

# Create the tarball from bin directory and give it a proper name
tar -czf $TARGET_NAME.tar.gz bin/ --transform s/bin/$TARGET_NAME/ 
echo $TARGET_NAME.tar.gz generated

# Clean up the mess created in the bin/ directory
rm bin/README.md
rm bin/wallbreaker-launcher.sh
rm -r bin/lib32


#!/bin/bash
mkdir ../build
cd ../build
qmake ..
make

sudo mkdir /usr/share/git-blog-editor
mkdir ~/.config/git-blog-editor

sudo cp -rf ./* /usr/share/git-blog-editor/

cd ../install
cp -f ./git-blog-editor.ico ~/.config/git-blog-editor/
sudo cp -f ./git-blog-editor.desktop /usr/share/applications

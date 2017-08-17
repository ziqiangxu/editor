#!/bin/bash
mkdir ../build
cd ../build
qmake ..
make

sudo mkdir /usr/share/git-blog-editor
mkdir ~/.config/git-blog-editor

sudo cp -rf mymainwindow /usr/share/git-blog-editor/

cd ../install
sudo cp -f ./editor.jpg /usr/share/icons/hicolor/scalable/apps
sudo cp -f ./git-blog-editor.desktop /usr/share/applications

echo "Copying VM configuration..."
sed "s|VPATH|$PWD|g" VMImages/base/base.vbox.template > VMImages/base/base.vbox
sed "s|VPATH|$PWD|g" VirtualBox/VirtualBox.xml.template > VirtualBox/VirtualBox.xml
rm -rf $HOME/.config/VirtualBox
cp -r VirtualBox $HOME/.config/

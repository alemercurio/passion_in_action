sudo apt-get install libstdc++6:i386

sudo add-apt-repository ppa:fpoussin/ppa
sudo apt-get update
sudo apt-get install qstlink2
wget https://raw.githubusercontent.com/mobyfab/QStlink2/master/res/49-stlinkv2.rules
sudo mv 49-stlinkv2.rules /etc/udev/rules.d
sudo chown root:root /etc/udev/rules.d/49-stlinkv2.rules

sudo usermod -a -G dialout `id -un`
sudo apt-get install gtkterm

wget https://miosix.org/toolchain/MiosixToolchainInstaller.run
sh MiosixToolchainInstaller.run

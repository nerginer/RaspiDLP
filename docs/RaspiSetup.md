First notes
I started with noobs os and instal it. After first boot I open the gui config tool and select not to boot to startx. I also enable the I2C and
SPI. 
sudo ifconfig to learn its ip and I reboot the pi.
Now instalation of the pre-required software.
ssh pi@raspiIPaddress
password:raspberry

ssh key and mount pi as a directory on my mac and installing the node from source this tutorial is great

https://www.youtube.com/watch?v=J6g53Hm0rq4

important commands

scp id_rsa.pub pi@192.168.1.108:.ssh/authorized_keys

sshfs pi@192.168.1.108: DLPpi



1- Install Nodejs 
the code from :
http://www.andrewconnell.com/blog/setup-node-js-on-raspberry-pi-2-b


sudo apt-get update

sudo apt-get upgrade

wget http://node-arm.herokuapp.com/node_latest_armhf.deb

sudo dpkg -i node_latest_armhf.deb



to understand if it instaled correctly
```sh
node -v
```

**Faye Instalation**
make a directory called faye under main directory DLP
copy fave_server.js
```sh
sudo npm install faye --save
node fave_server.js
```
**Faye Server Auto start**
which node
This gave me :

/usr/local/bin/node
Open crontab config:

sudo crontab -e
Then in my crontab :

@reboot sudo /usr/local/bin/node /home/pi/DLP/faye/fave_server.js & 
Save, reboot

** Install code repo**
```sh
sudo git clone https://github.com/nerginer/RaspiDLP.git
```

**Install Python dependencies**
install pubsub
```sh
sudo easy_install -Z pypubsub
```
install zope.interface
download source from
https://pypi.python.org/pypi/zope.interface/4.1.3#downloads
sudo python setup.py install

install python dev
sudo apt-get install python-dev

install twisted
download source from
http://twistedmatrix.com/trac/
sudo python setup.py install

install python-bayeux-client
https://github.com/dkmadigan/python-bayeux-client.git
sudo python setup.py install

** Screen Blank**
sudo nano /etc/kbd/config

Change these two lines.

\# screen blanking timeout. monitor remains on, but the screen is cleared to
\# range: 0-60 min (0==never) kernels I've looked at default to 10 minutes.
\# (see linux/drivers/char/console.c)
BLANK_TIME=0 (Was 30)

\# Powerdown time. The console will go to DPMS Off mode POWERDOWN_TIME
\# minutes _after_ blanking. (POWERDOWN_TIME + BLANK_TIME after the last input)
POWERDOWN_TIME=0 (I think it was 15)

Re start the file or just reboot
sudo /etc/init.d/kbd restart

\*****************************


**DIR Structure**

-DLP
  -faye
    fave_server.js
    
    

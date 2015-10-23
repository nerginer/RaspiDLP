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


**DIR Structure**

-DLP
  -faye
    fave_server.js
    
    

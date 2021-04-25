# Character-device-drivers-CaesarCipher
## Install
- `$sudo apt install make`
- `$sudo apt install build-essential -y`
- `$sudo apt install linux-headers-$(uname -r)`
- `$sudo apt-get install manpages-dev`


## Bắt đầu
Mở termial:
  - `$ gedit Makefile`
  - `$ gedit Kbuild`
  - `$ gedit driver_module.c`
  - `$ gedit user_test.c`
  - `$ make`
  - `$ sudo insmod driver_module.ko`
  - `$ sudo dmesg`
  - `$ sudo gcc user_test.c -o user_test`
  - `$ sudo ./user_test`

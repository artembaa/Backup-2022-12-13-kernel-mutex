debugfs: vm_area_struct, dentry

0. 
sudo mount -t debugfs none ~/sys/kernel/debug
sudo mount | grep  debugfs 
->none on /sys/kernel/debug type debugfs (rw)

1.
На Ubuntu 14 (32bit), core version 3.19.0-80-generic:
sudo apt-get install build-essential linux-headers-`uname -r`
sudo apt update && sudo apt upgrade

2.
Let’s get the new software packages we’ll need for this endeavour:
sudo apt install gcc make build-essential libncurses-dev exuberant-ctags

3. 
make
-> lkm.ko 

4. (каждый раз)
sudo insmod lkm.ko
sudo dmesg   // читаем журнал сообщений ядра
lsmod | grep "lkm"   // проверка наличия модуля в памяти
sudo rmmod lkmuname -r 

5. (каждый раз)
ROOT access to sys/kernel/debug:
	sudo chmod a+rx /sys/kernel/debug
	sudo chmod a+rx /sys/kernel/debug/lkm_debug2
	(действует только на один сеанс)
или
	https://stackoverflow.com/questions/31168179/how-can-i-allow-any-user-to-read-files-from-sys-kernel-debug
	(действует навсегда, после перезагрузки)

6. (каждый раз)
make для lkmusr
./lkmusr_prg с параметрами



KERNEL MUTEX
https://embetronicx.com/tutorials/linux/device-drivers/linux-device-driver-tutorial-mutex-in-linux-kernel/

DEBUGFS
https://www.opensourceforu.com/2010/10/debugging-linux-kernel-with-debugfs/

КАК РАЗБИРАТЬ ФАЙЛЫ .h
4.0.4 version
https://elixir.bootlin.com/linux/v3.19/source/include/linux/types.h#L131

КАК УЗНАТЬ ЛЮБОЙ PID
take pid list/tree
pstree -p

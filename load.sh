bin=./BUILD/KL25Z/GCC_ARM/semestralka-vytah.bin
dir=/tmp/mbed_device
mkdir $dir
mount /dev/$1 $dir
cp $bin $dir
umount $dir
rm -rf $dir

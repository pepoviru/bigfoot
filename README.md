bigfoot
=======

To create a dummy file:

dd if=/dev/zero of=zeros.bin bs=$((8*3600*1000)) count=$((25*12))

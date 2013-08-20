bigfoot
=======

To create a dummy file (12 rows, 25 hours sampled at 1KHz using doubles)

dd if=/dev/zero of=zeros.bin bs=$((8*3600*1000)) count=$((25*12))

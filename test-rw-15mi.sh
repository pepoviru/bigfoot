#!/bin/bash
echo "file,nrows,ncols,numelems,offset,from,to,treal,tuser,tsys"
for i in {0..96}
do
	#time -f "\t%E real,\t%U user,\t%S sys"
	/usr/bin/time -f ",%E,%U,%S" bftozeros /scratch/foodata/zeros.bin 12 $((25*3600*1000)) $((12*3600*1000)) 0 $(($i*900*1000)) $((($i+1)*900*1000-1)) $i
done

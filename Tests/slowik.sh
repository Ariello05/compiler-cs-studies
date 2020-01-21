#!/bin/sh
printf 'program0.mr\t';           ./maszyna-wirtualna-cln program0.mr <<< '65537'
printf 'program1.mr\t';           ./maszyna-wirtualna-cln program1.mr 
printf 'program2.mr\t';           ./maszyna-wirtualna-cln program2.mr <<< '481583025'
printf 'test0.mr\t';		./maszyna-wirtualna-cln test0.mr <<< '0 0 0 0 0 0 0 0'
printf 'test0.mr\t';		./maszyna-wirtualna-cln test0.mr <<< '1 -1 1 -1 2 -1 1 -2'
printf 'index.mr\t';		./maszyna-wirtualna-cln index.mr <<< '244941'
printf '1-numbers.mr\t';		./maszyna-wirtualna-cln 1-numbers.mr <<< '15'
printf '4-factorial.mr\t';		./maszyna-wirtualna-cln 4-factorial.mr <<< '7'
#scan

#!/bin/sh
printf '00-div-mod-mr\t';         ./maszyna-wirtualna-cln 00-div-mod.mr <<< '33 7'
printf '0-div-mod-mr\t';          ./maszyna-wirtualna-cln 0-div-mod.mr <<< '1 0'         
printf '1-numbers.mr\t';          ./maszyna-wirtualna-cln 1-numbers.mr <<< '15'
printf '2-fib.mr\t';              ./maszyna-wirtualna-cln 2-fib.mr <<< '1'
printf '3-fib-factorial.mr\t';    ./maszyna-wirtualna-cln 3-fib-factorial.mr <<< '20'
printf '4-factorial.mr\t';        ./maszyna-wirtualna-cln 4-factorial.mr <<< '20'
printf '5-tab.mr\t';              ./maszyna-wirtualna-cln 5-tab.mr 
printf '6-mod-mult.mr\t';         ./maszyna-wirtualna-cln 6-mod-mult.mr <<< '1234567890 1234567890987654321 987654321'
printf '7-loopiii.mr\t';          ./maszyna-wirtualna-cln 7-loopiii.mr <<< '0 0 0'
printf '7-loopiii.mr\t';          ./maszyna-wirtualna-cln 7-loopiii.mr <<< '1 0 2'
printf '8-for.mr\t';              ./maszyna-wirtualna-cln 8-for.mr <<< '12 23 34'
printf '9-sort.mr\t';             ./maszyna-wirtualna-cln 9-sort.mr
printf 'program0.mr\t';           ./maszyna-wirtualna-cln program0.mr <<< '1345601'
printf 'program1.mr\t';           ./maszyna-wirtualna-cln program1.mr 
printf 'program2.mr\t';           ./maszyna-wirtualna-cln program2.mr <<< '12345678901'
printf 'program2.mr\t';           ./maszyna-wirtualna-cln program2.mr <<< '12345678903'
printf 'error0.mr\t';             ./maszyna-wirtualna-cln error0.mr
printf 'error1.mr\t';             ./maszyna-wirtualna-cln error1.mr
printf 'error2.mr\t';             ./maszyna-wirtualna-cln error2.mr
printf 'error3.mr\t';             ./maszyna-wirtualna-cln error3.mr
printf 'error4.mr\t';             ./maszyna-wirtualna-cln error4.mr
printf 'error5.mr\t';             ./maszyna-wirtualna-cln error5.mr
printf 'error6.mr\t';             ./maszyna-wirtualna-cln error6.mr
printf 'error7.mr\t';             ./maszyna-wirtualna-cln error7.mr
printf 'error8.mr\t';             ./maszyna-wirtualna-cln error8.mr
printf 'test0.mr\t';		./maszyna-wirtualna-cln test0.mr <<< '0 0 0 0 0 0 0 0'
printf 'test1a.mr\t';		./maszyna-wirtualna-cln test1a.mr <<< '10'
printf 'test1b.mr\t';		./maszyna-wirtualna-cln test1b.mr <<< '10'
printf 'test1c.mr\t';		./maszyna-wirtualna-cln test1c.mr <<< '10'
printf 'test1d.mr\t';		./maszyna-wirtualna-cln test1d.mr <<< '10'
printf 'test2.mr\t';		./maszyna-wirtualna-cln test2.mr
#scan

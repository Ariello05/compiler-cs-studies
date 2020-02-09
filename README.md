# compiler-cs-studies

## About
Compiler for JFTT course (formal languages and translation theory).
Compiler target as well as language are both defined by dr.Gębala, specifications can be found in file specifications.pdf.


### Built with:
- [flex](https://github.com/westes/flex), as lexer
- [bison](https://www.gnu.org/software/bison/), as parser 

## Usage
Installation is straight forward, use
```
make
```
output will be ./scan, exectuable used to compile from .imp to .mr, use it like
```
./scan program.imp output.mr
```
Where .imp is imperative language, .mr is result virtual machine code (asm) both defined in file specifications.pdf.
In order to run .mr, you have to install "maszyna-wirtualna", which can be found in subfolder ./tests/maszynawirtualna.
Simply make it. Two files will be created: maszyna-wirtualna and maszyna-wirtualna-cln, where -cln uses [cln](https://www.ginac.de/CLN/) library. Use like:
```
./maszyna-wirtualna output.mr
```
For more details look into specifications.pdf

## Authors
- Paweł Dychus (Ariello05) - compiler
- dr. Maciej Gębala - specifications and virtual machine (maszyna wirtualna)

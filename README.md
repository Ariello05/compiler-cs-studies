# compiler-cs-studies

## About
Compiler for JFTT course (formal languages and translation theory).
Compiler target as well as language are both defined by dr.Gębala, specifications can be found in file [specifications.pdf](https://github.com/Ariello05/compiler-cs-studies/blob/master/specifications.pdf)

### Built with:
- [flex](https://github.com/westes/flex) - as lexer
- [bison](https://www.gnu.org/software/bison/) - as parser 
- [cln](https://www.ginac.de/CLN/) - used in virtual machine, cln version

## Installation and usage
### Compiler
Installation is straight forward (provided you have libs above), use
```
make
```
output will be ./scan, exectuable used to compile from .imp to .mr, use it like
```
./scan program.imp output.mr
```

### Virtual Machine
Where .imp is imperative language, .mr is result virtual machine code (asm).
In order to run .mr, you have to install "maszyna-wirtualna", which can be found in subfolder ./tests/maszynawirtualna.
Simply make it. Two files will be created: maszyna-wirtualna and maszyna-wirtualna-cln, where -cln uses [cln](https://www.ginac.de/CLN/) library. Use like:
```
./maszyna-wirtualna output.mr
```
For more details look into [specifications.pdf](https://github.com/Ariello05/compiler-cs-studies/blob/master/specifications.pdf)

### Test shell scripts
In folder Tests there can be found two scripts
* ./compile.sh - compiles .imp files into .mr using scan executable
* ./run.sh - runs .mr using maszyna-wirtualna executable

## Authors
* **Paweł Dychus** - *Compiler* - [Ariello05](https://github.com/Ariello05)
* **dr. Maciej Gębala** - *Specifications and virtual machine (maszyna wirtualna)*

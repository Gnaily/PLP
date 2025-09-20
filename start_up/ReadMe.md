## Start up——A Simple Sexpr 

A Sexpr is one of 
- name 
- number
- sequence of whitespace splited Sexpr wrapped in ( and )

### A Parser
parse the simple S expression

### Name Count
count the number of name in an S expression

## Modern C++ Features Used
- primitive data: bool、float、int、char、
- compound data:string、string_view、array, list
- user defined （compound）data: class (inherit、abstract、rtti)
- variable:reference、smart pointer (raii)
- traverse:loop/(stl)iterate/recursion

## Build
run the Makefile

build
 ```
 make
 ```

 clean
 ```
 make clean
 ```

## Run 
### Run Tests
run the shell script test.sh 
```
cd tests
./test.sh
```
### Run Executable
after build, run the shell script run.sh 
```
./run.sh
```

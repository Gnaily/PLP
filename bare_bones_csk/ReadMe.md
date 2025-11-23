# Bare Bones
## S Expression
A Sexpr is one of 
- name 
- number
- sequence of whitespace splited Sexpr wrapped in ( and )

### A Parser
parse the simple S expression

## The Bare Bones Language 
###  Grammer
```Program    ::= (Statement* Expression)

   Statement  ::= (Variable = Expression)

               | (if0 Expression Block Block)

               | (while0 Expression Block)

   Block      ::= Statement

               | (block Statement+)

   Expression ::= Number

               | Variable

               | (Variable + Variable)

               | (Variable / Variable)

               | (Variable == Variable)
```
## Ast Parser

parse Sexpr to Ast+

Ast+ = Ast + Error

## CSK Machine


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

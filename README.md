# The Lang Programming Language
## Build
## Run
use the command
`./main`
### Print a string
```
PRINT "Hello, World!"
```
### Basic Calculator (It doesn't work yet)
```
N1 := F (INPUT "Enter first number)
OPER := C (INPUT "Choose an operation ('+', '-', '*', '/'))
N2 := F (INPUT "Enter second number)
result = (IF (EQ (OPER, '+'), SUM,
    IF (EQ (OPER, '-'), DIFF,
        IF (EQ (OPER, '*'), MUL,
            IF (EQ (OPER, '/'), DIV, {
                RETURN := "Operation error"
            })
        )
    )
)) (N1, N2)
PRINT result
```
## Learn
### Variables
#### How to declare a variable?
```
s: S
s = "hello"
```
#### Equivalent in C++
```
string s;
s = "hello";
```
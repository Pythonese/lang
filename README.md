# The Lang Programming Language
## Build
## Run
use the command
`./main`
### Print a string
PRINT "Hello, World!"
### Basic Calculator 
N1 := F (INPUT "Enter first number)<br/>
OPER := C (INPUT "Choose an operation ('+', '-', '*', '/'))<br/>
N2 := F (INPUT "Enter second number)<br/>
result = (IF (EQ (OPER, '+'), SUM,<br/>
    IF (EQ (OPER, '-'), DIFF,<br/>
        IF (EQ (OPER, '*'), MUL,<br/>
            IF (EQ (OPER, '/'), DIV, {<br/>
                RETURN := "Operation error"<br/>
            })<br/>
        )<br/>
    )<br/>
)) (N1, N2)<br/>
PRINT result
## Learn
### Variables
#### How to declare a variable?
s: S<br/>
s = "hello"
#### Equivalent in C++
string s;<br/>
s = "hello";

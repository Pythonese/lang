# The Lang Programming Language
## Build
```
git clone https://github.com/Pythonese/lang
cmake .
make
```
## Run
```
./Lang input.txt
```
### Print a string
```
PRINT "Hello, World!"
```
### Create a function
```
fun: O
fun = {
    arg: I
    RETURN := (
        arg
        "hello"
    )
}
Q := fun -1
PRINT Q
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
#### How to declare a variable with automatic typing?
```
s := "hello"
```
#### How to declare a constant variable?
```
S1 := "hello"
```
The Lang has no keywords. It has only variables and operators.
#### Standard Variables
```
U, I, F, C, S, B, T, O,
PRINT, INPUT,
```
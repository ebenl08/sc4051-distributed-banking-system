# Java Client Setup, Compilation, and Execution Guide

## 1. Change Directory
First, navigate to the `client/` directory:

```sh
cd client/
```

## 2. Compile All Java Files
Compile all Java files in all subfolders and output the compiled `.class` files into a `bin/` directory. If `bin/` does not exist, create it:

```sh
mkdir -p bin
javac -d bin $(find . -name "*.java") # or just javac -d bin *.java handle/*.java net/*.java
```

This command finds all `.java` files in the current directory and its subdirectories, then compiles them into the `bin/` directory, preserving the package structure.

## 3. Run the Program
To execute a compiled class, use the following command from inside the `client/` directory. For example, to run the `DSBankingUI` class:

```sh
java -cp bin <module-name>
```


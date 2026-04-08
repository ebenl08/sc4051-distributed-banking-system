# ZeusBanking Client Setup


## 1. Compile All Java Files
Compile all Java files in all subfolders and output the compiled `.class` files into a `bin/` directory. If `bin/` does not exist, create it:

```sh
mkdir -p javabin
javac -d javabin $(find . -name "client/*.java") # or just javac -d bin *.java handle/*.java net/*.java
```

This command finds all `.java` files in the current directory and its subdirectories, then compiles them into the `bin/` directory, preserving the package structure.

## 2. Run the Program
To execute a compiled class, use the following command from inside the `client/` directory. For example, to run the `ZeusBankingClient` class:

```sh
java -cp javabin ZeusBankingClient --host <hostname> --port <port>
```

Alternatively you could also run the `run-client.sh` to open the actual CLI application or the `run-test-client.sh` to automatically load different request packages across the different services so that you don't need to spend time adding input into your application.


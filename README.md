# CS452 - P2: Simple Shell

This project is a C implementation of a simple shell. Leveraging system documentation, this project will apply computer science theory and software development fundamentals to produce a simple shell that will explore the system call interface and utilize system library code to replicate general shell functionality.

#### <u>NOTE</u>: All of the following commands must be performed in the root directory of this repository (i.e. the location of this README) in order to compile and run this program.

## Building

Before the program can be run, it needs to be compiled. To do so, run the following command:

```bash
make
```

#### After compilation is complete, the shell program itself can be run and interacted with in isolation by executing the following command in the terminal:

```bash
./myprogram
```

## Testing

The tests written to assess the correct functionality of the data structure itself can be run only after the program is initially compiled. To run these tests, use the following command:

```bash
make check
```

## Clean

To remove the current build of the program and prepare for a fresh compilation and build, you can run the following command in the terminal:

```bash
make clean
```

## Debug

The following command allows for the application of the AddressSanitizer memory leak debugger to run on the program to assist in trouble shooting memory leaks and program errors. To run add its functionality, it must be run immediately after the initial `make clean` command before any additional calls to run the program test check or the program itself. To do so, you can run the following command in the terminal: 

```bash
make debug
```

## Install Dependencies

In order to use all necessary program functionality, dependencies are needed.  Please run the following command to install them prior to attempting any program operation:

```bash
make install-deps
```

If you run into any issues with compilation, pursue this as a first attempt at troubleshooting.

<br></br>
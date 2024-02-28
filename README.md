# NeuralNetwork-C

- [Explanation](#explanation)
- [Building](#building)
- [Execution](#execution)
- [Todo](#todo)

Neural Network that reads handwritten numbers from the MNIST dataset written purely in C using only standard libraries.

This is a reworking of a project I did years ago, where I implemented a neural network purely in Java using only standard libraries.

That project was inspired by the Youtube series on [Neural Networks](https://www.youtube.com/playlist?list=PLZHQObOWTQDNU6R1_67000Dx_ZCJB-3pi) written by 3Blue1Brown.

As a way to get more proficient at C, I decided to rewrite my neural network project as an exercise in developing a slightly-larger-than-a-single-file-program in C.

## Explanation

The overall structure is based on a matrix datatype, consisting of 2-dimensional float arrays, and a neural network struct with accompanying functions that implement the functions a relatively simple neural network.

Included are feed-forward and a preliminary backpropagation implementation. The feed forward mechanism should be working fine, but applying the network cost in the 'training' process isn't quit working right at the moment. So far, the network will provide only random answers to test samples. I plan to fix this in time.

As a note to my future self, I should create a diagram and write out the math behind the neural network here as a complete description of how the network works.

Perhaps when I have this working sufficiently, I can explore GANs or CGAN implementations in C as well.

## Building

A 'Makefile' is provided. To build, run:

```
> make
```
A 'main' executable should be produced.

## Execution

> NOTE: Make sure you have a folder named 'data' that has two `.csv` files, `mnist_test.csv` and `mnist_train.csv`, in the same directory that you execute 'main'. These are the csv files that should contain the testing and training data respectively. If the files are not there or are named incorrectly, the program will complain that it cannot find the training and testing data.

Simple run the main executable:

```
> ./main
```

## Todo

Ideas for improvements that I have been thinking of:

- [ ] First off, get the network to train and try to achieve 94% accuracy with the MNIST data set.
- [ ] Determine a better way to test the network.
- [ ] Package the matrix functions into a shared library for future use.


# Operating Systems

Coursework for the Operating Systems course at ITESO. Autumn 2015.

## Setup

1. Install docker:

   OSX (assuming [Hombrew](http://brew.sh/) and [Hombrew-cask](http://caskroom.io/) are installed):
   ```
   $ brew cask install dockertoolbox
   ```

   For other operating systems, follow the instructions [here](https://docs.docker.com/installation/).

2. Start default docker machine:
   ```
   $ docker-machine start default
   ```

3. Get environment commands for the VM:
   ```
   $ docker-machine env default
   ```

4. Connect your shell to the `default` VM:
   ```
   $ eval "$(docker-machine env default)"
   ```

5. (Optional) Verify your setup:
   ```
   $ docker run hello-world
   ```

6. Clone this repo and cd into it:
   ```
   $ git clone https://github.com/miguelfrde/operatingsystems && cd operatingsystems
   ```

7. Build our container:
   ```
   $ docker build -t operating_systems .
   ```

8. Run the container and verify that everything is working correctly. Run the commands shown below, and you should get that output.

   ```
   $ docker run -v `pwd`/src:/osdev -ti operating_systems /bin/bash
   root@e7280caf55e8:/osdev# cd example/
   root@e7280caf55e8:/osdev/example# make
   gcc -O3 -g -Wall -Werror -D_USE_MATH_DEFINE -c main.c -o main.o
   gcc -O3 -g -Wall -Werror -D_USE_MATH_DEFINE -o main main.o
   root@e7280caf55e8:/osdev/example# ./main
   Operating systems, ITESO, Autumn 2015
   root@e7280caf55e8:/osdev/example# exit
   ```

10. Start coding!

There are two utility scripts (both should work on Linux and OSX):

1. `startdocker`: automates steps 2, 3 and 4. Run it as: `source scripts/startdocker`
2. `run`: automates the `docker run` command from step 9. Run it as: `./scripts/run`

## Coding

### Style guide

Follow the parts relevant to C from the [Google C++ style guide](http://google.github.io/styleguide/cppguide.html).

Some nice programming guidelines for C [here](https://github.com/btrask/stronglink/blob/master/SUBSTANCE.md)

### Git flow

When starting a new assignment or project, create a directory with a name relevant to the assignment in `src`.
When adding a feature or fix to that assignment, create a branch with a name relevant to the assignment and feature
and open a pull request to merge that branch to `master`. Let others check your code before merging the pull request.

### Makefiles

For simple makefiles, it's ok to use one like the [example one](src/example/Makefile).

For more complex ones, abstract it and generalize on a way similar to the one in
[miguelfrde/computergraphics/finalassignment](https://github.com/miguelfrde/computergraphics/blob/master/finalassignment/Makefile).

If you need to learn about Makefiles, [this](http://nuclear.mutantstargoat.com/articles/make/) is a very good resource.

# Operating Systems

Coursework for the Operating Systems course at ITESO. Autumn 2015.

## Setup

1. Install docker:

   OSX (assuming [Hombrew](http://brew.sh/) and [Hombrew-cask](http://caskroom.io/) are installed): `brew cask install dockertoolbox`

   For other operating systems, follow the instructions [here](https://docs.docker.com/installation/).

2. Start default docker machine: `docker-machine start default`

3. Get environment commands for the VM: `docker-machine env default`

4. Connect your shell to the `default` VM: `eval "$(docker-machine env default)"`

5. Verify your setup: `docker run hello-world`

6. Clone this repo and cd into it: `git clone https://github.com/miguelfrde/operatingsystems && cd operatingsystems`

7. Build our container: `docker build -t operating_systems .`

8. Run the container: `docker run operating_systems`

9. Start coding!

There's a utility script that automates steps 2, 3 and 4. It can be executed in the following way: `source scripts/startdocker`

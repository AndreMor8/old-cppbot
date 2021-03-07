Made with [aegis.cpp](https://github.com/zeroxs/aegis.cpp)

It was made to work on Linux systems.

# Docker

Recursively clone the repository, fill the .env and simply do:
>```bash
>docker build --tag andremor/cppbot .
>docker run andremor/cppbot
>
And voila, you will have the bot running.

# Build

**Prepare all CMake build environments with option `-DCMAKE_CXX_STANDARD=17`**

Example

>```bash
>cmake .. -DCMAKE_CXX_STANDARD=17
>```

* Install MongoDB C and C++ driver to the system.
* 
http://mongoc.org/libmongoc/current/installing.html

http://mongocxx.org/mongocxx-v3/installation/linux/

* Recursively clone `aegis.cpp` repository

>```bash
> git clone --recursive https://github.com/zeroxs/aegis.cpp
>```

* Install its dependencies, and install it.

>```bash
> cd aegis.cpp
> ./install-deps.sh
>```
When finished, a command will be displayed on the screen. Copy it and run it.

* Recursively clone this project

>```bash
> git clone --recursive https://github.com/AndreMor955/cppbot
> cd cppbot
>```

* Compile the bot

>```bash
> mkdir build
> cd build
> cmake ..
> cmake --build .
>```

* Fill `.env`
>```bash
> mv .env.example .env
> nano .env
>```

* Execute this bot
>```bash
> ./bin/cppbot
>```
It's recommended to do it from the main folder of the project, so that it correctly reads the `.env`.

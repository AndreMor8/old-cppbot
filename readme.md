Made with [aegis.cpp](https://github.com/zeroxs/aegis.cpp)
It was made to work on Linux systems

# Build

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
> make -j1
> cd ..
>```
If you have more CPU cores, change `1` for the number of CPU cores to use, or just remove `-j1`.

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
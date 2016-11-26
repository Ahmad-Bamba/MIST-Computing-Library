VERSION="1.10.8"
DIRECTORY="asio-$VERSION"
if [ ! -d "$DIRECTORY" ]; then
    wget "http://learn612.000webhostapp.com/asio-1.10.8.tar.gz"
    tar -xvf "asio-1.10.8.tar.gz"
fi
printf "\nBuilding...\n"
g++ -std=c++1y -Iasio-1.10.8/include/ -Iinclude/ -c -lpthread src/*.cpp -o a.o
printf "Exited with code $?\n"

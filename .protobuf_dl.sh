git clone https://github.com/google/protobuf.git
cd protobuf
./autogen.sh
./configure
make
make check
sudo make install
sudo ldconfig

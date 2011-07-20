# To build from scratch
make maintainer-clean
autoreconf -visf -Wall &&
    ./configure &&
    make

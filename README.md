QCloud is a general Qt Library that provides access to all kinds of open API network
disk/storage, such as dropbox, wuala and so on.

In order to provides general usage, it can also launch a daemon and interact application
with DBus interface.

To compile it you need cmake and Qt, QOAuth and QCA2.

in the source dir

$ mkdir build
$ cd build
$ cmake ..
$ make

To install as root:

# make install

This will install in the default prefix: /usr/local

If you want to change the prefix, call cmake with
 -DCMAKE_INSTALL_PREFIX=/my/prefix/

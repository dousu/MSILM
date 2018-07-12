FROM alpine:3.8
ARG base='ftp://gcc.gnu.org/pub/gcc/infrastructure'
RUN apk update \
  && apk upgrade \
  && apk add --no-cache make \
  && apk add --no-cache --virtual .build-dep gcc g++ paxmark bison flex texinfo gawk zip gmp-dev mpfr-dev mpc1-dev zlib-dev isl-dev linux-headers !gettext-dev gcc-doc libatomic libgcc libgomp libstdc++ make file \
  && mkdir sources \
  && cd sources \
  && wget -O- https://www.musl-libc.org/releases/musl-1.1.19.tar.gz | tar zxf - \
  && cd musl-1.1.19 \
  && ./configure \
  && make > /dev/null \
  && make install > /dev/null \
  && cd ../ \
  && wget -O- http://ftp.gnu.org/gnu/m4/m4-1.4.17.tar.gz | tar zxf - \
  && cd m4-1.4.17 \
  && ./configure \
  && make > /dev/null \
  && make install > /dev/null \
  && cd ../ \
  && wget -O- ${base}/gmp-6.1.0.tar.bz2 | tar jxf - \
  && cd gmp-6.1.0 \
  && ./configure \
  && make > /dev/null \
  && make check > /dev/null \
  && make install > /dev/null \
  && cd ../ \
  && wget -O- ${base}/mpfr-3.1.4.tar.bz2 | tar jxf - \
  && cd mpfr-3.1.4 \
  && ./configure \
  && make > /dev/null \
  && make check > /dev/null \
  && make install > /dev/null \
  && cd ../ \
  && wget -O- ${base}/isl-0.16.1.tar.bz2 | tar jxf - \
  && cd isl-0.16.1 \
  && ./configure \
  && make > /dev/null \
  && make install > /dev/null \
  && cd ../ \
  && wget -O- ${base}/mpc-1.0.3.tar.gz | tar zxf - \
  && cd mpc-1.0.3 \
  && ./configure \
  && make > /dev/null \
  && make check > /dev/null \
  && make install > /dev/null \
  && cd ../ \
  && wget -O- ftp://sourceware.org/pub/newlib/newlib-3.0.0.tar.gz | tar zxf - \
  && cd newlib-3.0.0 \
  && mkdir ../buildnlib \
  && cd ../buildnlib \
  && ../newlib-3.0.0/configure \
  && make > /dev/null \
  && make install > /dev/null \
  && cd ../ \
  && wget -O- http://ftpmirror.gnu.org/gcc/gcc-7.3.0/gcc-7.3.0.tar.xz | tar Jxf - \
  && cd gcc-7.3.0 \
  && mkdir ../buildgcc \
  && cd ../buildgcc \
  && ../gcc-7.3.0/configure --with-linker-hash-style=gnu --disable-libssp --disable-libmpx --disable-libmudflap --disable-libsanitizer --disable-symvers --disable-libgomp --disable-libatomic --disable-libitm --with-system-zlib --enable-languages=c,c++ --enable-cloog-backend --enable-default-pie --enable-__cxa_atexit --disable-werror --disable-nls --disable-multilib --disable-libstdcxx-pch --disable-fixed-point --enable-checking=release --with-pkgversion="Alpine gcc-7.3.0" --infodir=/usr/share/info --mandir=/usr/share/man --prefix=/usr \
  && make \
  && make install \
  && cd ../../ \
  && rm -rf sources

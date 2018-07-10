FROM alpine:3.8
ARG base='ftp://gcc.gnu.org/pub/gcc/infrastructure'
RUN apk add --no-cache make \
  && apk add --no-cache --virtual .build-dep gcc g++ gmp-dev file texinfo \
  && mkdir sources; cd sources \
  && wget -O- https://www.musl-libc.org/releases/musl-1.1.19.tar.gz | tar zxf -; cd musl-1.1.19 \
  && ./configure \
  && make > /dev/null \
  && make install > /dev/null; cd ../ \
  && wget -O- http://ftp.gnu.org/gnu/m4/m4-1.4.17.tar.gz | tar zxf -; cd m4-1.4.17 \
  && ./configure \
  && make > /dev/null \
  && make install > /dev/null; cd ../ \
  && wget -O- ${base}/gmp-6.1.0.tar.bz2 | tar jxf -; cd gmp-6.1.0 \
  && ./configure \
  && make > /dev/null \
  && make check > /dev/null \
  && make install > /dev/null; cd ../ \
  && wget -O- ${base}/mpfr-3.1.4.tar.bz2 | tar jxf - ; cd mpfr-3.1.4 \
  && ./configure \
  && make > /dev/null \
  && make check > /dev/null \
  && make install > /dev/null; cd ../ \
  && wget -O- ${base}/isl-0.16.1.tar.bz2 | tar jxf -; cd isl-0.16.1 \
  && ./configure \
  && make > /dev/null \
  && make install > /dev/null; cd ../ \
  && wget -O- ${base}/mpc-1.0.3.tar.gz | tar zxf -; cd mpc-1.0.3 \
  && ./configure \
  && make > /dev/null \
  && make check > /dev/null \
  && make install > /dev/null \
  && cd ../ \
  && wget -O- http://ftp.tsukuba.wide.ad.jp/software/gcc/releases/gcc-7.3.0/gcc-7.3.0.tar.xz | tar Jxf -; cd gcc-7.3.0 \
  && mkdir ../build; cd ../build \
  && ../gcc-7.3.0/configure --enable-language=c,c++ --disable-multilib --disable-bootstrap \
  && make all-gcc > /dev/null \
  && make all-target-libgcc > /dev/null \
  && make install-gcc > /dev/null \
  && make install-target-libgcc > /dev/null \
  && rm -rf sources

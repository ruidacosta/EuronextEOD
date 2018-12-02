FROM ubuntu:latest as builder

RUN apt-get -y update && apt-get install -y

RUN apt-get -y install build-essential sqlite3 libsqlite3-dev

RUN mkdir -p /root/src/app
RUN mkdir -p /root/app
WORKDIR /root/src/app

COPY . /root/src/app

RUN ./configure --destdir=/root/app
RUN make install

FROM ubuntu:latest

RUN \
    apt-get -y update \
    && apt-get install -y \
    && apt-get -y install python python-pip gettext-base \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*
RUN pip install requests

WORKDIR /root/
COPY --from=builder /opt/euronextEOD .

CMD ["./starter"]
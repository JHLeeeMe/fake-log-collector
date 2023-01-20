FROM alpine:latest
LABEL maintainer="JHLeeeMe"

ARG JAVA_VERSION=17
ARG SCALA_VERSION=2.13
ARG KAFKA_VERSION=3.3.1

RUN apk add --no-cache tzdata && \
    cp /usr/share/zoneinfo/Asia/Seoul /etc/localtime && \
    echo "Asia/Seoul" > /etc/timezone && \
    apk del tzdata

RUN apk add --no-cache bash tree wget vim
RUN apk add --no-cache openjdk${JAVA_VERSION}

WORKDIR /opt

# kafka_2.13-3.3.1
RUN wget "https://downloads.apache.org/kafka/${KAFKA_VERSION}/kafka_${SCALA_VERSION}-${KAFKA_VERSION}.tgz"
RUN tar xzf kafka_${SCALA_VERSION}-${KAFKA_VERSION}.tgz && \
    rm *.tgz && \
    ln -s kafka_${SCALA_VERSION}-${KAFKA_VERSION} kafka

ENV KAFKA_HOME=/opt/kafka
ENV PATH=${PATH}:${KAFKA_HOME}/bin

WORKDIR /workspace

# COPY scripts
COPY bootstrap.sh /tmp/

# kafka server port
EXPOSE 9092

ENTRYPOINT ["bash", "/tmp/bootstrap.sh"]

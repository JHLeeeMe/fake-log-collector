FROM alpine:latest
LABEL maintainer="JHLeeeMe"

ARG JAVA_VERSION=17
ARG SCALA_VERSION=2.13
ARG KAFKA_VERSION=3.3.1

WORKDIR /opt

RUN apk add --no-cache bash \
                       tree \
                       openjdk${JAVA_VERSION} \
                       wget

# kafka_2.13-3.3.1
RUN wget "https://downloads.apache.org/kafka/${KAFKA_VERSION}/kafka_${SCALA_VERSION}-${KAFKA_VERSION}.tgz" && \
    tar xzf kafka_${SCALA_VERSION}-${KAFKA_VERSION}.tgz && \
    rm kafka_${SCALA_VERSION}-${KAFKA_VERSION}.tgz && \
    ln -s kafka_${SCALA_VERSION}-${KAFKA_VERSION} kafka

ENV KAFKA_HOME=/opt/kafka
ENV PATH=${PATH}:${KAFKA_HOME}/bin

RUN echo "alias ll='ls -alF'" >> /etc/profile

WORKDIR /workspace

# COPY scripts
COPY bootstrap.sh ./
RUN chown root.root ./*.sh && \
    chmod 700 ./*.sh

# kafka server port
EXPOSE 9092

ENTRYPOINT ["./bootstrap.sh"]

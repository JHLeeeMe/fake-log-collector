FROM alpine:latest
LABEL maintainer="JHLeeeMe"

ARG JAVA_VERSION=17
ARG PYTHON_VERSION=3.10.9-r1

RUN apk add --no-cache tzdata && \
    cp /usr/share/zoneinfo/Asia/Seoul /etc/localtime && \
    echo "Asia/Seoul" > /etc/timezone && \
    apk del tzdata

RUN apk add --no-cache bash tree wget vim git cmake make g++ libbsd-dev \
                       curl-dev spdlog-dev rapidjson-dev
RUN apk add --no-cache openjdk${JAVA_VERSION}
RUN apk add --no-cache python3=${PYTHON_VERSION} \
                       py3-pip
RUN apk add --no-cache librdkafka-dev

RUN ln -s /usr/lib/jvm/java-${JAVA_VERSION}-openjdk /opt/java
ENV JAVA_HOME=/opt/java
ENV PATH=${PATH}:${JAVA_HOME}/bin

COPY bootstrap.sh /tmp

WORKDIR /workspace

ENTRYPOINT ["bash", "/tmp/bootstrap.sh"]

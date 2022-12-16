FROM alpine:latest
LABEL maintainer="JHLeeeMe"

ARG JAVA_VERSION=17
ARG PYTHON_VERSION=3.10.9-r1

RUN apk add --no-cache bash tree wget \
                       openjdk${JAVA_VERSION} \
                       python3=${PYTHON_VERSION} \
                       py3-pip

RUN ln -s /usr/lib/jvm/java-${JAVA_VERSION}-openjdk /opt/java

ENV JAVA_HOME=/opt/java
ENV PATH=${PATH}:${JAVA_HOME}/bin

COPY bootstrap.sh /tmp
RUN chown root.root /tmp/bootstrap.sh && \
    chmod 700 /tmp/bootstrap.sh

WORKDIR /workspace

ENTRYPOINT ["/tmp/bootstrap.sh"]

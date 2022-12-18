FROM jhleeeme/ubuntu:18.04
LABEL maintainer="JHLeeeMe"

# install dev tools
RUN apt-get update && \
    apt-get -y upgrade && \
    apt-get install -y build-essential && \
    apt-get install -y software-properties-common && \
    apt-get install -y tree vim net-tools wget openssh-server openssh-client htop iputils-ping sudo git make curl man unzip rsync && \
    rm -rf /var/lib/apt/lists/*

# java
RUN mkdir -p /opt/java && \
    wget --no-check-certificate --no-cookies --header "Cookie: oraclelicense=accept-securebackup-cookie" http://download.oracle.com/otn-pub/java/jdk/8u131-b11/d54c1d3a095b4ff2b6607d096fa80163/jdk-8u131-linux-x64.tar.gz && \
    tar -xvf jdk-8u131-linux-x64.tar.gz -C /opt/java --strip-components=1 && \
    rm jdk-8u131-linux-x64.tar.gz

# ENV java
ENV JAVA_HOME /opt/java
ENV PATH $PATH:$JAVA_HOME/bin

RUN rm -rf /usr/bin/java && \
    ln -s $JAVA_HOME/bin/java /usr/bin/java

# passwordless ssh
RUN rm -f /etc/ssh/ssh_host_dsa_key /etc/ssh/ssh_host_rsa_key /root/.ssh/id_rsa && \
    ssh-keygen -q -N "" -t dsa -f /etc/ssh/ssh_host_dsa_key && \
    ssh-keygen -q -N "" -t rsa -f /etc/ssh/ssh_host_rsa_key && \
    ssh-keygen -q -N "" -t rsa -f /root/.ssh/id_rsa && \
    cp /root/.ssh/id_rsa.pub /root/.ssh/authorized_keys && \
    chmod 600 /root/.ssh/authorized_keys

# hadoop
RUN wget https://archive.apache.org/dist/hadoop/core/hadoop-3.2.1/hadoop-3.2.1.tar.gz && \
    tar -xzf hadoop-3.2.1.tar.gz -C /opt/ && \
    rm hadoop-3.2.1.tar.gz && \
    cd /opt/ && ln -s ./hadoop-3.2.1 hadoop

# ENV hadoop
ENV HADOOP_HOME /opt/hadoop
ENV HADOOP_COMMON_HOME $HADOOP_HOME
ENV HADOOP_HDFS_HOME $HADOOP_HOME
ENV HADOOP_MAPRED_HOME $HADOOP_HOME
ENV HADOOP_YARN_HOME $HADOOP_HOME
ENV HADOOP_CONF_DIR $HADOOP_HOME/etc/hadoop
ENV PATH $PATH:$HADOOP_HOME/bin:$HADOOP_HOME/sbin

RUN echo "export JAVA_HOME=$JAVA_HOME" >> $HADOOP_CONF_DIR/hadoop-env.sh && \
    echo "export HDFS_DATANODE_USER=root" >> $HADOOP_CONF_DIR/hadoop-env.sh && \
    echo "export HDFS_NAMENODE_USER=root" >> $HADOOP_CONF_DIR/hadoop-env.sh && \
    echo "export HDFS_SECONDARYNAMENODE_USER=root" >> $HADOOP_CONF_DIR/hadoop-env.sh && \
    echo "export YARN_RESOURCEMANAGER_USER=root" >> $HADOOP_CONF_DIR/yarn-env.sh && \
    echo "export YARN_NODEMANAGER_USER=root" >> $HADOOP_CONF_DIR/yarn-env.sh

RUN mkdir -pv $HADOOP_HOME/input && \
    mkdir -pv $HADOOP_HOME/dfs/name && \
    mkdir -pv $HADOOP_HOME/dfs/data && \
    mkdir -pv $HADOOP_HOME/tmp

RUN cp $HADOOP_HOME/etc/hadoop/*.xml $HADOOP_HOME/input

COPY config/hadoop/hdfs-site.xml $HADOOP_HOME/etc/hadoop/hdfs-site.xml
COPY config/hadoop/core-site.xml $HADOOP_HOME/etc/hadoop/core-site.xml
COPY config/hadoop/mapred-site.xml $HADOOP_HOME/etc/hadoop/mapred-site.xml
COPY config/hadoop/yarn-site.xml $HADOOP_HOME/etc/hadoop/yarn-site.xml

RUN $HADOOP_HOME/bin/hdfs namenode -format

COPY config/ssh/ssh_config /root/.ssh/config
RUN chmod 600 /root/.ssh/config && \
    chown root.root /root/.ssh/config

# workingaround docker.io build error
RUN ls -la $HADOOP_HOME/etc/hadoop/*-env.sh && \
    chmod +x $HADOOP_HOME/etc/hadoop/*-env.sh && \
    ls -la $HADOOP_HOME/etc/hadoop/*-env.sh

# fix the 254 error code
RUN sed  -i "/^[^#]*UsePAM/ s/.*/#&/"  /etc/ssh/sshd_config && \
    echo "UsePAM no" >> /etc/ssh/sshd_config && \
    echo "Port 2122" >> /etc/ssh/sshd_config

COPY config/hadoop/create-workers.sh /etc/create-workers.sh
COPY bootstrap.sh /etc/bootstrap.sh
RUN chown root.root /etc/bootstrap.sh /etc/create-workers.sh && \
    chmod 700 /etc/bootstrap.sh /etc/create-workers.sh

# HDFS ports
EXPOSE 50010 50020 50070 50075 50090 8020 9000

# Mapred ports
EXPOSE 10020 19888

# YARN ports
EXPOSE 8030 8031 8032 8033 8040 8042 8088

# Other ports
EXPOSE 49707 2122

ENTRYPOINT ["/etc/bootstrap.sh"]

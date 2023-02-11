# fake-log-collector
![fake-log-collector-pipeline](https://user-images.githubusercontent.com/31606119/218061194-35aee0cf-bec6-4527-b578-f1132febaa49.jpeg)

### 1. log-generator -> Kafka
Send `random log data(Nginx, Apache, Flask)` to kafka `raw` topic
```python
# Log Form e.g.
Nginx & Apache: 
    '$remote_addr - $remote_user [$time_local] '                            
    '"$request" $status $body_bytes_sent '                                  
    '"$http_referer" "$http_user_agent"'                                    
  e.g.                                                                      
    192.168.0.250 - - [14/Jan/2023:12:34:56 +0900]                          
    "GET / HTTP/1.1" 200 396                                                
    "-" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/57.0.2987.133 Safari/537.36"

Flask:
    [%(asctime)s] %(levelname)s in %(module)s: %(message)s [in %(pathname)s:%(lineno)d]
  e.g.                                                                      
    [2023-01-14 12:34:56] INFO in app: 58461 Andrew Fields Apt. 110 [in /path/to/app.py:123]
```
  - [src/python/log-generator](https://github.com/JHLeeeMe/fake-log-collector/tree/master/src/python/log-generator)

### 2. Transform (Using kafka streams)
  1. Transform data from `raw` topic  
  2. Send to `transformed` topic
```python
# Transformed Log Form e.g.
Nginx & Apache:
    key,datetime,ip,request,statusCode,bodyBytes
  e.g.
    nginx,2023-01-14T12:34:56+0900,192.168.0.250,GET / HTTP/1.1,200,396

Flask:
    key,datetime,logLevel,message
  e.g.
    flask,2023-01-14T12:34:56+0900,INFO,58461 Andrew Fields Apt. 110
```
  - [src/java/transformer](https://github.com/JHLeeeMe/fake-log-collector/tree/master/src/java/transformer)

### 3. Kafka -> Consumer -> Message Queue
  1. Consume data from `transformed` topic  
  2. Send to `message queues` (System V ipc)
  - [src/cpp/consumer-cpp](https://github.com/JHLeeeMe/fake-log-collector/tree/master/src/cpp/consumer-cpp)

### 4. Message Queue -> InfluxDB
  1. Receive data from `Message Queue`  
  2. Write to `influxdb's fake_log db` (using udp protocol)
  - [src/cpp/write-to-influxdb](https://github.com/JHLeeeMe/fake-log-collector/tree/master/src/cpp/write-to-influxdb)
  
### 5. Message Queue -> Hadoop HDFS
  1. Receive data from `Message Queue`  
  2. Save to `Hadoop HDFS` (using webHDFS)
  - [src/cpp/save-to-hdfs](https://github.com/JHLeeeMe/fake-log-collector/tree/master/src/cpp/save-to-hdfs)
  
### 6. Collect system metrics -> Influxdb (Using Telegraf)
  1. collect using `[[inputs.docker]]` plugin  
  2. write to `influxdb's container_metrics db` (using `[[outputs.influxdb]]` plugin)
  - [Dockers/telegraf/telegraf.conf](https://github.com/JHLeeeMe/fake-log-collector/blob/master/Dockers/telegraf/telegraf.conf)

### 7. Visualize InfluxDB (Using Grafana)
Visualize `fake_log` & `container_metrics` database

**Fake Logs Dashboard**
![Fake_Logs_Dashboard](https://user-images.githubusercontent.com/31606119/218243939-5a7d2777-114e-4e7d-980c-6ac72c945da3.jpg)

**Container Metrics Dashboard**
![Container_Metrics_Dashboard](https://user-images.githubusercontent.com/31606119/218243941-869dfca0-791c-4f45-92c4-b1062724e68b.jpg)

# Usage
### 1. git clone
```bash
git clone https://github.com/JHLeeeMe/fake-log-collector.git
```

### 2. Run
```bash
cd fake-log-collector/bin
./run.sh -d

# Check
docker-compose ps
```

### 3. Visualize with Grafana
Access `localhost:3000` with web-browser (grafana)  
(id: `admin`, password: `admin`)

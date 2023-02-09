# fake-log-collector
![fake-log-collector-pipeline](https://user-images.githubusercontent.com/31606119/217535524-9e1f5fc5-932a-4f05-83a8-694498c94fdc.jpeg)

### 1. log-generator -> Kafka
Send ```random log data(Nginx, Apache, Flask)``` to kafka ```raw``` topic
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
2-1. Transform data from ```raw``` topic  
2-2. Send to ```transformed``` topic
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

### 3. Kafka -> Consumer
3-1. Consume data from ```transformed``` topic  
3-2. Send to ```message queues``` (System V ipc)
  - [src/cpp/consumer-cpp](https://github.com/JHLeeeMe/fake-log-collector/tree/master/src/cpp/consumer-cpp)

### 4. Message Queue -> Influxdb
  - [src/cpp/write-to-influxdb](https://github.com/JHLeeeMe/fake-log-collector/tree/master/src/cpp/write-to-influxdb)
  
### 5. Message Queue -> Hadoop HDFS
  - [src/cpp/save-to-hdfs](https://github.com/JHLeeeMe/fake-log-collector/tree/master/src/cpp/save-to-hdfs)
  
### 6. Collect system metrics -> Influxdb (using Telegraf)
6-1. 

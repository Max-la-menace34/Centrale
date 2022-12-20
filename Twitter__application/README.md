# Tweetoscope22_GROUP-3_OLLIVIER1_LEVI2_RAILLAT3
## Abstract
The aim of this project is to develop a tool that collect hashtags from Twitter and to display the top 5 live. So Selim Ollivier and I have developped a CI/CD pipeline on Gitlab. All the code is in Java using kafka brokcer in order to send informations topic to topic. All this infrastucture is using docker images orchestrated by kubernetes. Pipelines may not work on github but work on Gitlab.

## Find Dockerimages on DockerHub

All the images are on maxlamenace34/tweetoscope<br>
If you want to pull an image :
```
docker pull maxlamenace34/tweetoscope:Nom_de_l'image(tag)
```
## Execute Docker images one by one :

#### Zookeeper and kafka :

You need to start the server and the different topics before starting launching docker :

```
docker run --rm -it --network host zookeeper
docker run --rm -it --network host --env KAFKA_BROKER_ID=0 --env KAFKA_LISTENERS=PLAINTEXT://:9092 --env KAFKA_ZOOKEEPER_CONNECT=localhost:2181 --env KAFKA_CREATE_TOPICS="Tweet:2:1,Filter:1:1,Extractor:1:1,Counter:1:1" wurstmeister/kafka
```

#### Producer 

If you want to run manually the image producer :
```
docker run --rm -it --network host maxlamenace34/tweetoscope:producer_sampled

```
or with database : 
```
docker run --rm -it --network host maxlamenace34/tweetoscope:producer_database
```

#### Filter 

If you want to run manually the image filter :

```
docker run --rm -it --network host maxlamenace34/tweetoscope:emptyfilter
```
or 
```
docker run --rm -it --network host maxlamenace34/tweetoscope:filter_langue
```


#### Extractor

If you want to run manually the image extractor :
```
docker run --rm -it --network host maxlamenace34/tweetoscope:HashtagExtractor
```

#### Counter 

If you want to run manually the image Cunter :
```
docker run --rm -it --network host maxlamenace34/tweetoscope:HashtagCounter2
```

#### Visualizor 

If you want to run manually the image Visual :
```
docker run --rm -it --network host maxlamenace34/tweetoscope:visualizor
```
leader est un argument que l'on donne au docker lors de son éxécution : leader étant le nom de cet argument

#### All the command at the same time:
```
docker run --rm -it --network host zookeeper
docker run --rm -it --network host --env KAFKA_BROKER_ID=0 --env KAFKA_LISTENERS=PLAINTEXT://:9092 --env KAFKA_ZOOKEEPER_CONNECT=localhost:2181 --env KAFKA_CREATE_TOPICS="Tweet:2:1,Filter:1:1,Extractor:1:1,Counter:1:1" wurstmeister/kafka
docker run --rm -it --network host maxlamenace34/tweetoscope:producer_sampled
docker run --rm -it --network host maxlamenace34/tweetoscope:emptyfilter
docker run --rm -it --network host maxlamenace34/tweetoscope:HashtagExtractor
docker run --rm -it --network host maxlamenace34/tweetoscope:HashtagCounter2
docker run --rm -it --network host maxlamenace34/tweetoscope:visualizator
$KAFKA_HOME/bin/kafka-console-consumer.sh --bootstrap-server localhost:9092 --topic Counter --from-beginning
```

## Report

Our report can be found on the [project's pages](https://maxime.raillat.pages-student.centralesupelec.fr/tweetoscope22_group-3_ollivier1_levi2_raillat3/report.pdf).

## Test Coverage Results

We perform a bunch of automatic tests on the DateTweetFilter module. The results are available [here](https://maxime.raillat.pages-student.centralesupelec.fr/tweetoscope22_group-3_ollivier1_levi2_raillat3/testCoverageReport/).

## Demonstration videos

- [demo code + run docker](https://www.youtube.com/watch?v=rQULeTE-ThI)
- [demo Kubernetes](https://www.youtube.com/watch?v=527E0Y99r-o)
- [demo CI/CD](https://www.youtube.com/watch?v=ZFrYSUxLYTo)


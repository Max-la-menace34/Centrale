/*
Copyright 2022 Sélim Ollivier, Maxime Raillat, Jérémie Levi

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>
 */
package tweetoscope;

import java.time.Duration;
import java.util.Collections;
import java.util.Properties;

import org.apache.kafka.clients.consumer.ConsumerConfig;
import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.apache.kafka.clients.consumer.ConsumerRecords;
import org.apache.kafka.clients.consumer.KafkaConsumer;
import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.ProducerConfig;

import com.twitter.twittertext.Extractor;

/**
 * Base class to configure a Kafka stream using a producer-consumer architecture.
 * The types of data exchanged need be parametrized through Type parameters <inputKeyType,inputValueType,outputKeyType,outputValueType>.
 * 
 * @author Sélim Ollivier, Maxime Raillat, Jérémie Levi
 **/
public abstract class Stream<inputKeyType,inputValueType,outputKeyType,outputValueType> {
    /*
	 * List of Kafka bootstrap servers. Example: localhost:9092,another.host:9092
	 * 
	 * @see:
	 * https://jaceklaskowski.gitbooks.io/apache-kafka/content/kafka-properties-
	 * bootstrap-servers.html
	 */
	protected String bootstrapServers;
	/*
	 * Name of the source Kafka topic
	 */
	protected String inputTopicName;
	/*
	 * Name of the destination Kafka topic
	 */
	protected String outputTopicName;
	/**
	 * Twitter lib utility object
	 */
	final Extractor twitterTextExtractor = new Extractor();

	protected KafkaConsumer<inputKeyType, inputValueType> consumer;
	protected KafkaProducer<outputKeyType, outputValueType> producer;

	/** 
	 * Constructor.
	 * 
	 * @param bootstrapServers servers where to fetch and send data (exemple: "localhost:9092").
	 * @param inputTopicName name of the topic where to fetch data (exemple: Filter).
	 * @param outputTopicName  name of the topic where to publish transformed/filtered data (exemple: Hashtags).
	 **/
	public Stream(String bootstrapServers, String inputTopicName, String outputTopicName) {
		this.bootstrapServers = bootstrapServers;
		this.inputTopicName = inputTopicName;
		this.outputTopicName = outputTopicName;
		
		this.consumer = new KafkaConsumer<inputKeyType, inputValueType>(
				configureKafkaConsumer(bootstrapServers));
		// reads from any partition of the topic
		consumer.subscribe(Collections.singletonList(inputTopicName));

		this.producer = new KafkaProducer<outputKeyType, outputValueType>(
				configureKafkaProducer(bootstrapServers));
    }

	/**  
	 * Launch stream service.
	 **/
    public void run() {
		try {
			Duration timeout = Duration.ofMillis(1000);
			while (true) {
				// reads events
				ConsumerRecords<inputKeyType, inputValueType> records = consumer.poll(timeout);
				for (ConsumerRecord<inputKeyType, inputValueType> record : records) {
					transformAndSend(record.key(), record.value());
				}
			}
		} catch (Exception e) {
			System.out.println("something went wrong... " + e.getMessage());
		} finally {
			consumer.close();
			producer.close();
		}
	}

	/**
	 * Transformation to perform on data before sending it to downstream topic
	 * 
	 * @param key
	 * @param tweetText
	 */
	protected abstract void transformAndSend(inputKeyType key, inputValueType tweetText);

	/**
	 * Prepares default configuration for the Kafka producer <Void, String> 
	 * If more sophistigated data is used (eg: <String, String>, <Void, Map<String,Integer>>), method overriding is needed to change Serializer/Deserializer.
	 * 
	 * @param bootstrapServers servers where to fetch and send data (exemple: "localhost:9092").
	 * 
	 * @return configuration properties for the Kafka producer
	 */
	protected Properties configureKafkaProducer(String bootstrapServers) {
		Properties producerProperties = new Properties();

		producerProperties.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, bootstrapServers);
		producerProperties.put(ProducerConfig.KEY_SERIALIZER_CLASS_CONFIG,
				org.apache.kafka.common.serialization.VoidSerializer.class.getName());
		producerProperties.put(ProducerConfig.VALUE_SERIALIZER_CLASS_CONFIG,
				org.apache.kafka.common.serialization.StringSerializer.class.getName());

		return producerProperties;
	}

	/**
	 * Prepares default configuration for the Kafka consumer <Void, String> 
	 * If more sophistigated data is used (eg: <String, String>, <Void, Map<String,Integer>>), method overriding is needed to change Serializer/Deserializer.
	 * 
	 * @param bootstrapServers servers where to fetch and send data (exemple: "localhost:9092").
	 * 
	 * @return configuration properties for the Kafka consumer
	 */
	protected Properties configureKafkaConsumer(String bootstrapServers) {
		Properties consumerProperties = new Properties();

		consumerProperties.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, bootstrapServers);
		consumerProperties.put(ConsumerConfig.KEY_DESERIALIZER_CLASS_CONFIG,
				org.apache.kafka.common.serialization.VoidDeserializer.class.getName());
		consumerProperties.put(ConsumerConfig.VALUE_DESERIALIZER_CLASS_CONFIG,
				org.apache.kafka.common.serialization.StringDeserializer.class.getName());
		consumerProperties.put(ConsumerConfig.GROUP_ID_CONFIG, "Consumer");
		consumerProperties.put(ConsumerConfig.AUTO_OFFSET_RESET_CONFIG, "latest"); // from beginning

		return consumerProperties;
	}
}

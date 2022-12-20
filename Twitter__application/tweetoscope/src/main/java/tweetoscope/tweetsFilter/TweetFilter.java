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
package tweetoscope.tweetsFilter;

import java.time.Duration;
import java.util.Collections;
import java.util.Properties;

import org.apache.kafka.clients.consumer.ConsumerConfig;
import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.apache.kafka.clients.consumer.ConsumerRecords;
import org.apache.kafka.clients.consumer.KafkaConsumer;
import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.ProducerConfig;
import org.apache.kafka.clients.producer.ProducerRecord;

import com.google.gson.Gson;
import com.twitter.clientlib.model.Tweet;

import tweetoscope.utils.TweetMapper;

/**
 * Reacts to the reception of a new Tweet, if the Tweet matches the filter
 * condition, downstream subscribers are notified, otherwise the process is
 * silent. Tweets are received from
 * {@link tweetoscope.tweetsProducer.TweetsProducer} via Kafka.
 * Filtered Tweets are passes down to the
 * {@link tweetoscope.HashtagCounter} via Kafka.
 * 
 * @author Sélim Ollivier, Maxime Raillat, Jérémie Levi
 *
 */
public abstract class TweetFilter extends Thread {

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
	/*
	 * Name of the filter
	 */
	protected String filterName;

	/**
	 * Constructor.
	 * 
	 * @param bootstrapServers list of Kafka bootstrap servers. Example:localhost:9092,another.host:9092
	 * @param inputTopicName name of the source Kafka topic
	 * @param outputTopicName name of the destination Kafka topic
	 * @param filterName Name of the filter to use.
	  */
	protected TweetFilter(String bootstrapServers, String inputTopicName, String outputTopicName, String filterName) {
		this.filterName = filterName;
		this.bootstrapServers = bootstrapServers;
		this.inputTopicName = inputTopicName;
		this.outputTopicName = outputTopicName;
	}

	/**
	 * Creates a Kafka consumer and a Kafka producer.
	 * The consumer reads a tweet from an upstream Kafka topic, checks if it matches a given condition and publishes it on the downstream topic
	 * (until the filter element is interrupted).
	 */
	@Override
	public void run() {

		KafkaConsumer<Void, String> consumer = new KafkaConsumer<Void, String>(
				configureKafkaConsumer(bootstrapServers));
		// reads from any partition of the topic
		consumer.subscribe(Collections.singletonList(inputTopicName));

		KafkaProducer<Void, String> producer = new KafkaProducer<Void, String>(
				configureKafkaProducer(bootstrapServers));

		try {
			Duration timeout = Duration.ofMillis(1000);
			while (true) {
				// reads events
				ConsumerRecords<Void, String> records = consumer.poll(timeout);
				for (ConsumerRecord<Void, String> record : records) {
					Gson gson = new Gson();
					Tweet tweet = gson.fromJson(record.value(), TweetMapper.class).toTweet();
					System.out.println("receiving tweet in filter");
					System.out.println(tweet.getText());
					if (match(tweet)) {
						producer.send(new ProducerRecord<Void, String>(outputTopicName, null, tweet.getText()));
					}
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
	 * Prepares default configuration for the Kafka producer <Void, String> 
	 * 
	 * @param bootstrapServers servers where to fetch and send data (exemple: "localhost:9092").
	 * 
	 * @return configuration properties for the Kafka producer
	 */
	private Properties configureKafkaProducer(String bootstrapServers) {
		Properties producerProperties = new Properties();

		producerProperties.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, bootstrapServers);
		producerProperties.put(ProducerConfig.KEY_SERIALIZER_CLASS_CONFIG,
				org.apache.kafka.common.serialization.VoidSerializer.class.getName());
		producerProperties.put(ProducerConfig.VALUE_SERIALIZER_CLASS_CONFIG,
				org.apache.kafka.common.serialization.StringSerializer.class.getName());

		return producerProperties;
	}

	/**
	 * Prepares configuration for the Kafka consumer <Void, String> 
	 * All filters share the "the_filters" consumer group.
	 * 
	 * @return configuration properties for the Kafka consumer
	 */
	private Properties configureKafkaConsumer(String bootstrapServers) {
		Properties consumerProperties = new Properties();

		consumerProperties.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, bootstrapServers);
		consumerProperties.put(ConsumerConfig.KEY_DESERIALIZER_CLASS_CONFIG,
				org.apache.kafka.common.serialization.VoidDeserializer.class.getName());
		consumerProperties.put(ConsumerConfig.VALUE_DESERIALIZER_CLASS_CONFIG,
				org.apache.kafka.common.serialization.StringDeserializer.class.getName());
		consumerProperties.put(ConsumerConfig.GROUP_ID_CONFIG, "the_filters");
		consumerProperties.put(ConsumerConfig.AUTO_OFFSET_RESET_CONFIG, "earliest"); // from beginning

		return consumerProperties;
	}

	/**
	 * Tests the filter condition.
	 * 
	 * @param tweet Tweet to examine
	 * 
	 * @return true if the Tweet complies with the filter, false if it doesn't match
	 *         the filter conditions
	 */
	public abstract boolean match(Tweet tweet);

}
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
package tweetoscope.tweetsProducer;

import java.util.Properties;

import tweetoscope.utils.TweetMapper;
import tweetoscope.utils.TweetSerializer;

import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.ProducerConfig;

/**
 * Introduces Tweets into the system. The Tweets might be crafted ones, Tweets
 * read from an archive, or Tweets retrieved live from the Twitter API.
 * 
 * @author Sélim Ollivier, Maxime Raillat, Jérémie Levi
 *
 */
public abstract class TweetsProducer {
	/*
	 * Kafka producer
	 */
	protected KafkaProducer<Void, TweetMapper> kafkaProducer;
	/*
	 * List of Kafka bootstrap servers. Example: localhost:9092,another.host:9092
	 * 
	 * @see:
	 * https://jaceklaskowski.gitbooks.io/apache-kafka/content/kafka-properties-
	 * bootstrap-servers.html
	 */
	protected static String bootstrapServers;
	/*
	 * Name of the destination Kafka topic
	 */
	protected static String topicName;

	/**
	 * Constructor.
	 * 
	 * @param bootstrapServers servers where to fetch and send data (exemple: "localhost:9092").
	 * @param topicName name of the topic where to fetch extracted hashtags (exemple: Hashtags).
	 **/
	public TweetsProducer(String bootstrapServers, String topicName) {
		TweetsProducer.bootstrapServers = bootstrapServers;
		TweetsProducer.topicName = topicName;
	}

	/**
	 * Prepares configuration for the Kafka producer <Void, TweetMapper> 
	 * 
	 * @return configuration properties for the Kafka producer
	 */
	protected Properties configureKafkaProducer() {
		Properties producerProperties = new Properties();
		producerProperties.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, bootstrapServers);
		producerProperties.put(ProducerConfig.KEY_SERIALIZER_CLASS_CONFIG,
				"org.apache.kafka.common.serialization.VoidSerializer");
		producerProperties.put(ProducerConfig.VALUE_SERIALIZER_CLASS_CONFIG,
				TweetSerializer.class);
		return producerProperties;
	}
}	


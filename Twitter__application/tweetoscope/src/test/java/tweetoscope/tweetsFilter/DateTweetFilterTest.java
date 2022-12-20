package tweetoscope.tweetsFilter;

import static org.junit.Assert.*;

import java.time.Duration;
import java.time.OffsetDateTime;
import java.time.ZoneOffset;
import java.util.Collections;
import java.util.Properties;

import org.apache.kafka.clients.admin.Admin;
import org.apache.kafka.clients.admin.AdminClientConfig;
import org.apache.kafka.clients.admin.CreateTopicsResult;
import org.apache.kafka.clients.admin.NewTopic;
import org.apache.kafka.clients.consumer.ConsumerConfig;
import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.apache.kafka.clients.consumer.ConsumerRecords;
import org.apache.kafka.clients.consumer.KafkaConsumer;
import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.ProducerConfig;
import org.apache.kafka.clients.producer.ProducerRecord;
import org.junit.Test;

import com.twitter.clientlib.model.Tweet;

import tweetoscope.utils.TweetMapper;
import tweetoscope.utils.TweetSerializer;

public class DateTweetFilterTest {

	private Tweet tweet1;
	private Tweet tweet2;

	private KafkaProducer<Void, TweetMapper> producer;
	private KafkaConsumer<Void, String> consumer;

	private String inputTopicName = "TweetTest";
	private String outputTopicName = "FilterTest";
	private String bootstrapServers = "localhost:9092";

	private DateTweetFilter dateTweetFilter;

	public DateTweetFilterTest() {
		this.tweet1 = new Tweet();
		tweet1.setId("001");
		tweet1.setCreatedAt(OffsetDateTime.of(2017, 6, 20, 11, 46, 23, 0, ZoneOffset.UTC));
		tweet1.setText("Choisissez un #travail que vous aimez et vous n'aurez pas a travailler un seul jour de votre vie");
		tweet1.setAuthorId("31");
		tweet1.setConversationId("01");
		tweet1.getGeo();
		tweet1.setLang("fr");

		this.tweet2 = new Tweet();
		tweet2.setId("002");
		tweet2.setCreatedAt(OffsetDateTime.of(2022, 6, 20, 11, 48, 20, 0, ZoneOffset.UTC));
		tweet2.setText("Si on travaille pour gagner sa vie, pourquoi se tuer au #travail ?");
		tweet2.setAuthorId("31");
		tweet2.setConversationId("01");
		tweet2.getGeo();
		tweet2.setLang("fr");


		this.dateTweetFilter = new DateTweetFilter(
				"localhost:9092", "TweetTest", 
				"FilterTest", "2021-02-03T10:15:30+01:00");
	}

	@Test
	public void testMatch() {
		assertEquals(true, dateTweetFilter.match(tweet1));
		assertEquals(false, dateTweetFilter.match(tweet2));
	}

	@Test
	public void testDateTweetFilter() {

		Properties properties = new Properties();
		properties.put(AdminClientConfig.BOOTSTRAP_SERVERS_CONFIG, bootstrapServers);
	
		Admin admin = Admin.create(properties);

	    int partitions = 1;
    	short replicationFactor = 1;
		NewTopic newInputTopic = new NewTopic(inputTopicName, partitions, replicationFactor);
		NewTopic newOutputTopic = new NewTopic(outputTopicName, partitions, replicationFactor);

		admin.createTopics(Collections.singleton(newInputTopic));
		admin.createTopics(Collections.singleton(newOutputTopic));
		
		dateTweetFilter.start();

		PublishConsume(tweet1);
		PublishConsume(tweet2);

		dateTweetFilter.stop();

		admin.deleteTopics(Collections.singleton(inputTopicName));
		admin.deleteTopics(Collections.singleton(outputTopicName));
	}

	protected void PublishConsume(Tweet tweet) {

		producer = new KafkaProducer<Void, TweetMapper>(
				configureKafkaProducer(bootstrapServers));
		consumer = new KafkaConsumer<Void, String>(
				configureKafkaConsumer(bootstrapServers));
		consumer.subscribe(Collections.singletonList(outputTopicName));

		TweetMapper tweetMapper = new TweetMapper(tweet);
		producer.send(new ProducerRecord<Void,TweetMapper>(inputTopicName, tweetMapper));

		try {
			Duration timeout = Duration.ofMillis(1000);
			long start = System.currentTimeMillis();
			while (start - System.currentTimeMillis()>=-10000) { // Waiting messages for 1 minute 
				// reads events
				ConsumerRecords<Void, String> records = consumer.poll(timeout);
				for (ConsumerRecord<Void, String> record : records) {
					assertEquals(tweet.getText(), record.value());
					assertEquals(true, dateTweetFilter.match(tweet));
				}
			}
			System.out.println(start - System.currentTimeMillis());
		} catch (Exception e) {
			System.out.println("something went wrong... " + e.getMessage());
		} finally {
			consumer.close();
			producer.close();
		}
	}

	/**
	 * Prepares configuration for the Kafka producer <Void, TweetMapper> 
	 * 
	 * @return configuration properties for the Kafka producer
	 */
	protected Properties configureKafkaProducer(String bootstrapServers) {
		Properties producerProperties = new Properties();
		producerProperties.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, bootstrapServers);
		producerProperties.put(ProducerConfig.KEY_SERIALIZER_CLASS_CONFIG,
				"org.apache.kafka.common.serialization.VoidSerializer");
		producerProperties.put(ProducerConfig.VALUE_SERIALIZER_CLASS_CONFIG,
				TweetSerializer.class);
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
}

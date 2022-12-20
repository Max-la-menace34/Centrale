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

import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.Map.Entry;
import java.util.stream.Collectors;

import org.apache.kafka.clients.producer.ProducerConfig;
import org.apache.kafka.clients.producer.ProducerRecord;

import tweetoscope.utils.LeaderBoardSerializer;

/**
 * 
 * Reacts to the reception of a new hashtag by updating how many times it has
 * been seen so far, and sending to its subscribers the updated list of the most
 * popular ones.
 * <p>
 * Hashtags are received via Java Flow from the upstream component
 * {@link tweetoscope.HashtagExtractor}. Downstream component
 * ({@link tweetoscope.Visualiazor}) is notified of the new leader
 * board data via Java Flow.
 * 
 * @author Sélim Ollivier, Maxime Raillat, Jérémie Levi
 *
 */
public final class HashtagCounter extends Stream<Void,String,Void,Map<String, Integer>> {
	/**
	 * Number of lines to include on the leader board
	 */
	protected int nbLeaders;

	/**
	 * Map <Hashtag text - number of occurrences>
	 */
	protected Map<String, Integer> hashtagOccurrenceMap;

	/**
	 * List of most popular hashtags, used to checked if the list is changed after a
	 * new hashtag is received
	 */
	protected Map<String, Integer> previousLeaderMap;

	/**
	 * Main method.
	 * 
	 * @param args command line arguments.
	 **/
	public static void main(String[] args) {
		new HashtagCounter(args[0], args[1], args[2], Integer.valueOf(args[3]));
	}

	/**
	 * @param bootstrapServers servers where to fetch and send data (exemple: "localhost:9092").
	 * @param inputTopicName name of the topic where to fetch extracted hashtags (exemple: Hashtags).
	 * @param outputTopicName  name of the topic where to publish LeaderBoard (exemple: Top).
	 * @param nbLeaders number of hashtags to include on the leader board
	 **/
	public HashtagCounter(String bootstrapServers, String inputTopicName, String outputTopicName, int nbLeader) {
		super(bootstrapServers, inputTopicName, outputTopicName);
		this.nbLeaders = nbLeader;

		hashtagOccurrenceMap = new HashMap<String, Integer>();

		run();
	}

	/**
	 * Counts and select the most cited hashtags by updating the the topHashtagsMaps.
	 * 
	 * @param keyVoid null key
	 * @param hashtag 
	  */
	@Override
	public void transformAndSend(Void keyVoid, String hashtag) {
		synchronized (hashtagOccurrenceMap) {
			// avoid ConcurrentModificationException when multiple sources are used
			// simultaneously

			// inserts the new tag or increments the number of occurrences if it's not a new
			// tag
			String key = "#" + hashtag;
			if (hashtagOccurrenceMap.containsKey(key)) {
				hashtagOccurrenceMap.replace(key, 1 + hashtagOccurrenceMap.get(key));
			} else {
				hashtagOccurrenceMap.put(key, 1);
			}

			// sorts by number of occurrences and keeps only the top ones
			Map<String, Integer> topHashtagsMap = hashtagOccurrenceMap.entrySet().stream()
					.sorted(Collections.reverseOrder(Map.Entry.comparingByValue())).limit(nbLeaders)
					.collect(Collectors.toMap(Map.Entry::getKey, Map.Entry::getValue));

			List<Entry<String, Integer>> topHashtagsMap2 = hashtagOccurrenceMap.entrySet().stream()
					.sorted(Collections.reverseOrder(Map.Entry.comparingByValue())).limit(nbLeaders)
					.collect(Collectors.toList());

			// notifies the subscribers
			if (previousLeaderMap == null || !previousLeaderMap.equals(topHashtagsMap)) {
				producer.send(new ProducerRecord<Void, Map<String, Integer>>(outputTopicName, null, topHashtagsMap));
				
				previousLeaderMap = topHashtagsMap;

				// print the top hashtags
				System.out.println("-".repeat(80));
				System.out.println("-".repeat(80));
				System.out.println(" ".repeat(29) + "Most Popular Hashtags" + " ".repeat(29));
				System.out.println("-".repeat(80));
				int i = 1;
				for (Map.Entry mapentry : topHashtagsMap2) {
					System.out.println("#" + String.valueOf(i) + " - " + mapentry.getKey() + "  -  " + String.valueOf(mapentry.getValue()));
				}
				System.out.println("");
			}
			// else the top list is not changed, no need to re-publish it

			
		}
	}

	/**  
	 * Override Kakfa Producer configuration to be able to send Map<String,Integer> values.
	 * 
	 * @param bootstrapServers servers where to fetch and send data (exemple: "localhost:9092").
	 * 
	 * @return configuration properties for the Kafka producer
	 **/
	@Override
	protected Properties configureKafkaProducer(String bootstrapServers) {
		Properties producerProperties = new Properties();

		producerProperties.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, bootstrapServers);
		producerProperties.put(ProducerConfig.KEY_SERIALIZER_CLASS_CONFIG,
				org.apache.kafka.common.serialization.VoidSerializer.class.getName());
		producerProperties.put(ProducerConfig.VALUE_SERIALIZER_CLASS_CONFIG,
				LeaderBoardSerializer.class);

		return producerProperties;
	}
}
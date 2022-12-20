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

import java.util.List;

import org.apache.kafka.clients.producer.ProducerRecord;

import com.twitter.twittertext.Extractor;

/**
 * Reacts to the reception of a new Tweet by parsing its text to look for
 * hashtags. For each hashtag found, if any, notifies its subscribers. Tweets
 * are received from the upstream component
 * {@link tweetoscope.tweetsFilter.TweetFilter} via Kafka.
 * Hashtags are passed downstream to the
 * {@link tweetoscope.HashtagCounter} via Kafka.
 * 
 * @author Sélim Ollivier, Maxime Raillat, Jérémie Levi
 *
 */
public final class HashtagExtractor extends Stream<Void,String,Void,String> {

	/**
	 * Twitter lib utility object
	 */
	final Extractor twitterTextExtractor = new Extractor();

	/**
	 * Main method.
	 * 
	 * @param args command line arguments.
	 **/
	public static void main(String[] args) {
		new HashtagExtractor(args[0], args[1], args[2]);
	}

	/** 
	 * Constructor.
	 * 
	 * @param bootstrapServers servers where to fetch and send data (exemple: "localhost:9092").
	 * @param inputTopicName name of the topic where to fetch filtered tweets (exemple: Filter).
	 * @param outputTopicName  name of the topic where to publish extracted hashtags (exemple: Hashtags).
	 **/
	public HashtagExtractor(String bootstrapServers, String inputTopicName, String outputTopicName) {
		super(bootstrapServers, inputTopicName, outputTopicName);
		run();
	}

	/**
	 * Send tweet's hashtags to downstream topic.
	 */
	@Override
	public void transformAndSend(Void key, String value) {
		if (value != null) {
			// extracts the hashtags in the Tweet
			List<String> hashtags = twitterTextExtractor.extractHashtags(value);

			// passes down the hashtags to the subscribers
			for (String hashtag : hashtags) {
				producer.send(new ProducerRecord<Void, String>(outputTopicName, null, hashtag));
			}
		}
	}
}
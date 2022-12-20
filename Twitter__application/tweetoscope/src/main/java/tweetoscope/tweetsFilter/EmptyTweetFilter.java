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

import com.twitter.clientlib.model.Tweet;

/**
 * Empty Tweet filter: all Tweets are accepted
 * 
 * @author Sélim Ollivier, Maxime Raillat, Jérémie Levi
 *
 */
public final class EmptyTweetFilter extends TweetFilter {

	/**
	 * Main mathod.
	 * 
	 * @param args command line arguments
	 **/
	public static void main(String[] args) {
        EmptyTweetFilter emptyTweetFilter =  new EmptyTweetFilter(args[0], args[1], args[2]);
		emptyTweetFilter.start();
    }

	/**
	 * Creates an empty filter that tests nothing.
	 * 
	 * Creates a Kafka consumer and a Kafka producer.
	 * The consumer reads a tweet from an upstream Kafka topic, checks if it matches a given condition and publishes it on the downstream topic
	 * (until the filter element is interrupted).
	 * 
	 * @param bootstrapServers list of Kafka bootstrap servers. Example:localhost:9092,another.host:9092
	 * @param inputTopicName name of the source Kafka topic
	 * @param outputTopicName name of the destination Kafka topic
	 */
	public EmptyTweetFilter(String bootstrapServers, String inputTopicName, String outputTopicName) {
		super(bootstrapServers, inputTopicName, outputTopicName, "EmptyTweetFilter");
	}

	/** 
	 * Empty filter always return true.
	 * 
	 * @return true.
	 */
	@Override
	public boolean match(Tweet tweet) {
		return true;
	}
}

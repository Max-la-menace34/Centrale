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

import com.twitter.clientlib.TwitterCredentialsBearer;
import com.twitter.clientlib.api.TwitterApi;

/**
 * The filters in this family require a connection to the Twitter API to perform
 * their tests.
 * 
 * @author Sélim Ollivier, Maxime Raillat, Jérémie Levi
 *
 */
public abstract class OnlineTweetFilter extends TweetFilter {
	/**
	 * token to access Twitter API endpoint, get from the environment variables
	 */
	protected static String BEARER_TOKEN;
	/**
	 * Twitter API instance
	 */
	public static TwitterApi twitterApiInstance;

	/**
	 * Constructors builds a filter and instanciate Twitter API.
	 * 
	 * @param bootstrapServers list of Kafka bootstrap servers. Example:localhost:9092,another.host:9092
	 * @param inputTopicName name of the source Kafka topic
	 * @param outputTopicName name of the destination Kafka topic
	 * @param filterName Name of the filter to use.
	  */
	protected OnlineTweetFilter(String bootstrapServers, String inputTopicName, String outputTopicName, String filterName) {
		super(bootstrapServers, inputTopicName, outputTopicName, filterName);
		// gets the Twitter access token from environment variables
		BEARER_TOKEN = System.getenv("BEARER_TOKEN");
		if (BEARER_TOKEN == null) {
			System.err.println("There was a problem getting your bearer token."
					+ " Please make sure you set the BEARER_TOKEN environment variable");
			System.exit(-1);
		}

		twitterApiInstance = new TwitterApi();
		TwitterCredentialsBearer credentials = new TwitterCredentialsBearer(BEARER_TOKEN);
		twitterApiInstance.setTwitterCredentials(credentials);
	}
}

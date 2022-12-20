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

/**
 * Forwards Tweets retrieved live from the Twitter API.
 * <p>
 * Important note: Requires the BEARER_TOKEN environment variable to be set to
 * connect to the Twitter API endpoint.
 * 
 * @author Sélim Ollivier, Maxime Raillat, Jérémie Levi
 *
 */
public abstract class OnlineTweetsProducer extends TweetsProducer {

	/**
	 * token to access Twitter API endpoint
	 */
	protected static String BEARER_TOKEN;

	/**
	 * Constructor.
	 * 
	 * @param bootstrapServers servers where to fetch and send data (exemple: "localhost:9092").
	 * @param topicName name of the topic where to fetch extracted hashtags (exemple: Hashtags).
	 **/
	OnlineTweetsProducer(String bootstrapServers, String topicName) {
        super(bootstrapServers, topicName);
		// gets the Twitter access token from environment variables
		BEARER_TOKEN = System.getenv("BEARER_TOKEN");
		if (BEARER_TOKEN == null) {
			System.err.println("There was a problem getting your bearer token."
					+ " Please make sure you set the BEARER_TOKEN environment variable");
			System.exit(-1);
		}
	}
}
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

import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;

import com.twitter.clientlib.ApiException;
import com.twitter.clientlib.model.SingleTweetLookupResponse;
import com.twitter.clientlib.model.Tweet;

/**
 * Filters Tweets according to their country code
 * 
 * @author Sélim Ollivier, Maxime Raillat, Jérémie Levi
 *
 */
public class CountryCodeTweetFilter extends OnlineTweetFilter {

	/**
	 * The targeted country code to match
	 */
	protected String countryCode;
	
	/**
	 * Main method.
	 * 
	 * @param args command line arguments.
	 **/
	public static void main(String[] args) {
        CountryCodeTweetFilter countryCodeTweetFilter =  new CountryCodeTweetFilter(args[0], args[1], args[2], args[3]);
		countryCodeTweetFilter.start();
    }

	/**
	 * Creates a filter that queries the Twitter API to retrieve the country code
	 * associated with the "place" tag of a Tweet (if it is set) and tests whether
	 * its value equals a given code.
	 * 
	 * @param bootstrapServers servers where to fetch and send data (exemple: "localhost:9092").
	 * @param inputTopicName name of the topic where to fetch tweets published by the producer (exemple: Tweets).
	 * @param outputTopicName  name of the topic where to publish filtered tweets (exemple: Filter).
	 * @param countryCode targeted country code to match (example: "us").
	 */
	public CountryCodeTweetFilter(String bootstrapServers, String inputTopicName, String outputTopicName, String countryCode) {
		super(bootstrapServers, inputTopicName, outputTopicName, "CountryCodeFilter");
		this.countryCode = countryCode;
	}

	/**
	 * Checks if the tweet was posted in the selected country.
	 * 
	 * @param tweet tweet to filter.
	 * 
	 * @return if the tweet matchs the condition.
	 **/
	@Override
	public boolean match(Tweet tweet) {
		String tweetCountryCode = getCountryCode2(tweet);
		return (tweetCountryCode != null) && (tweetCountryCode.equals(countryCode));
	}

	/**
	 * Returns the country code of the place tagged in a Tweet, if any. Version 2:
	 * using the findTweetId function of the Twitter API
	 * 
	 * @param tweet
	 * 
	 * @return the country code of the place tagged in a Tweet if the information is
	 *         available, null otherwise
	 */
	private String getCountryCode2(Tweet tweet) {
		if ((tweet.getGeo() != null) && (tweet.getGeo().getPlaceId() != null)) {
			try {
				Set<String> expansions = new HashSet<>(Arrays.asList("geo.place_id"));
				Set<String> tweetFields = new HashSet<>(
						Arrays.asList("created_at", "text", "author_id", "conversation_id", "geo", "lang"));
				Set<String> userFields = null;
				Set<String> mediaFields = null;
				Set<String> placeFields = new HashSet<>(Arrays.asList("country_code"));
				Set<String> pollFields = null;
				SingleTweetLookupResponse response = twitterApiInstance.tweets().findTweetById(tweet.getId(),
						expansions, tweetFields, userFields, mediaFields, placeFields, pollFields);
				if (response.getIncludes() != null && response.getIncludes().getPlaces() != null) {
					return response.getIncludes().getPlaces().get(0).getCountryCode();
				}
			} catch (ApiException e) {
				System.err.println("error while retreiving the country code: " + e.getResponseBody());
			}
		}
		return null;
	}

}

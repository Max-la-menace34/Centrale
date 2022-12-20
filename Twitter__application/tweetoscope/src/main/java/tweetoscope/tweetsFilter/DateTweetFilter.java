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

import java.time.OffsetDateTime;

import com.twitter.clientlib.model.Tweet;

/**
 * Filters Tweets according to the date of publication.
 * 
 * @author Sélim Ollivier, Maxime Raillat, Jérémie Levi
 *
 */
public class DateTweetFilter extends TweetFilter {

	/**
	 * Ending date to consider
	 */
	protected OffsetDateTime date;

    /**
	 * Main mathod.
	 * 
	 * @param args command line arguments
	 **/
    public static void main(String[] args) {
        DateTweetFilter dateTweetFilter =  new DateTweetFilter(args[0], args[1], args[2], args[3]);
		dateTweetFilter.start();
    }

	/**
	 * Creates a filter that tests whether the creation date of a the tweet's author account - reference as the "created_at" tag - is before the one specified.
	 * 
	 * Creates a Kafka consumer and a Kafka producer.
	 * The consumer reads a tweet from an upstream Kafka topic, checks if it matches a given condition and publishes it on the downstream topic
	 * (until the filter element is interrupted).
     * 
     * @param bootstrapServers list of Kafka bootstrap servers. Example:localhost:9092,another.host:9092
	 * @param inputTopicName name of the source Kafka topic
	 * @param outputTopicName name of the destination Kafka topic
	 * @param date Ending date to consider. Exemple: "2017-02-03T10:15:30+01:00"
	 */
	public DateTweetFilter(String bootstrapServers, String inputTopicName, String outputTopicName, String date) {
        super(bootstrapServers, inputTopicName, outputTopicName, "DateFilter");
		this.date = OffsetDateTime.parse(date);
	}

    /**
	 * Checks the tweet's author creation date.
	 * 
	 * @return if it matchs the condition.
	  */
    @Override
    public boolean match(Tweet tweet) {
        
        OffsetDateTime tweetDate = tweet.getCreatedAt();
        if (tweetDate==null) {
            tweetDate = OffsetDateTime.now();}

        return tweetDate.compareTo(date)<0;
    }
}

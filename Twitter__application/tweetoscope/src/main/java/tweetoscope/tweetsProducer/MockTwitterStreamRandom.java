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

import java.util.Random;

import com.twitter.clientlib.model.Tweet;

import tweetoscope.utils.TweetMapper;


/**
 * Mimics the TwitterStreamSampleReaderSingleton class. To be used when the
 * Twitter sampled stream rate limit is exceeded for instance. Creates a
 * continuous stream of random Tweets.
 * 
 * @author Sélim Ollivier, Maxime Raillat, Jérémie Levi
 *
 */
public final class MockTwitterStreamRandom extends OfflineTweetsProducer {

	/*
	 * Name of the destination Kafka topic.
	 */
	protected String topicName;

	/**
	 * Main mathod.
	 * 
	 * @param args command line arguments
	 **/
	public static void main(String[] args) throws InterruptedException {
		new MockTwitterStreamRandom(args[0], args[1]);
	}

	/**
	 * Creates a new MockTwitterStreamRandom.
	 * 
	 * Posts random Tweets to the downstream Kafka
	 * topic. A random Tweet text is a concatenation of hashtags chosen from a
	 * reduced set. The Tweet is also assigned a language among a reduced list.
	 * 
	 * @param bootstrapServers servers where to fetch and send data (exemple: "localhost:9092").
	 * @param topicName name of the topic where to fetch extracted hashtags (exemple: Hashtags).
	 */
	public MockTwitterStreamRandom(String bootstrapServers, String topicName) throws InterruptedException {
        super(bootstrapServers, topicName);
		String[] hashtags = { "fun", "bitCoin", "climate", "crypto", "CS", "Metz", "weather", "summer", "holidays",
				"health", "running", "sport" };
		String[] languages = { "fr", "en", "ru", "es", "it" };

		Tweet tweet;
		int nb = 0;
		String text;
		Random r = new Random();
		while (true) {
			// crafts a random Tweet
			nb++;
			tweet = new Tweet();
			text = "Tweet " + nb;
			for (int i = 0; i < (int) (4 * Math.random()); i++) {
				double d;
				do {
					d = r.nextGaussian();
					d = (int) (hashtags.length / 2 + d * hashtags.length / 2);
				} while (d < 0 || d > hashtags.length - 1);
				text += "#" + hashtags[(int) d] + " ";
			}
			tweet.setId("" + nb);
			tweet.setText(text);
			tweet.setLang(languages[(int) (Math.random() * languages.length)]);

			TweetMapper[] tweetMappers = new TweetMapper[1];
			tweetMappers[0] = new TweetMapper(tweet);
			sendTweets(tweetMappers);
			// publishes the Tweet
			
			// waits for a while
			try {
				Thread.sleep(10);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
    }
}
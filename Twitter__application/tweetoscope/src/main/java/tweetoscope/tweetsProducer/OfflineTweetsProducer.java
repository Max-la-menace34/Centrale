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

import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.ProducerRecord;

import tweetoscope.utils.TweetMapper;

/**
 * Produces Tweets without connecting to the Twitter API (recorded or crafted
 * Tweets)
 * 
 * @author Sélim Ollivier, Maxime Raillat, Jérémie Levi
 *
 */
public abstract class OfflineTweetsProducer extends TweetsProducer {

    /**
	 * Constructor.
	 * 
	 * @param bootstrapServers servers where to fetch and send data (exemple: "localhost:9092").
	 * @param topicName name of the topic where to fetch extracted hashtags (exemple: Hashtags).
	 **/
    public OfflineTweetsProducer(String bootstrapServers, String topicName) {
        super(bootstrapServers, topicName);
        }
    
    /** 
     * Sends tweets to Kafka topic
     * 
     * @param tweetMappers tweets to publish.
     **/
    protected void sendTweets(TweetMapper[] tweetMappers) {
        try {
			kafkaProducer = new KafkaProducer<Void, TweetMapper>(configureKafkaProducer());

            for (int i=0; i<tweetMappers.length; i++) {
                kafkaProducer.send(new ProducerRecord<Void,TweetMapper>(topicName, tweetMappers[i]));
                Thread.sleep(1);
            }
		} catch (Exception e) {
			System.err.println("something went wrong... " + e.getMessage());
		} finally {
			kafkaProducer.close();
		}
    } 
    };

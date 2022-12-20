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

import java.io.*;
import java.nio.file.*;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.core.json.JsonReadFeature;
import com.fasterxml.jackson.databind.DeserializationFeature;
import com.fasterxml.jackson.databind.ObjectMapper;

import tweetoscope.utils.TweetMapper;


/**
 * Reads and publishes tweets from a .txt file.
 * 
 * @author Sélim Ollivier, Maxime Raillat, Jérémie Levi
 *
 */
public final class MockTwitterStreamRecorded extends OfflineTweetsProducer {

	/*
	 * Name of the destination Kafka topic.
	 */
	protected String topicName;
    /* 
     * Path where to fetch recorded tweets.
     */
    private Path filePath = Path.of(System.getProperty("user.dir") + "//TestBases//largeTestBase.txt");

    /**
	 * Main mathod.
	 * 
	 * @param args command line arguments
	 **/
    public static void main(String[] args) {
		new MockTwitterStreamRecorded(args[0], args[1]);
	}

    /**
	 * Constructor.
	 * 
	 * @param bootstrapServers servers where to fetch and send data (exemple: "localhost:9092").
	 * @param topicName name of the topic where to fetch extracted hashtags (exemple: Hashtags).
	 */
    public MockTwitterStreamRecorded(String bootstrapServer, String topicName) {
        super(bootstrapServer, topicName);
        // creates the Kafka producer with the appropriate configuration
		sendTweets(getTweetMappers());
    }

	/**
	 * Fetch tweets from given database and posts them to the java flow.
     * 
     * @return TweetMapper[]
	 */
	protected TweetMapper[] getTweetMappers () {

        String jsonArrayString = null;
        try {
            jsonArrayString = Files.readString(filePath).replace("{\"tweets\": ", "");
            jsonArrayString = jsonArrayString.substring(0,jsonArrayString.length()-1);
        } catch (IOException e) {
            e.printStackTrace();
        }

        ObjectMapper objectMapper = new ObjectMapper();
        objectMapper.configure(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false);
        objectMapper.configure(JsonReadFeature.ALLOW_UNESCAPED_CONTROL_CHARS.mappedFeature(), true);
        objectMapper.configure(DeserializationFeature.USE_JAVA_ARRAY_FOR_JSON_ARRAY, true);

        TweetMapper[] tweetMappers = null;
        try {
            tweetMappers = objectMapper.readValue(jsonArrayString, TweetMapper[].class);
        } catch (JsonProcessingException e) {
            e.printStackTrace();
        }

        return tweetMappers;
    }
}

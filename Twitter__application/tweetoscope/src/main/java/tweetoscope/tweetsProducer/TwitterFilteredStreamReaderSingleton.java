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

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.URISyntaxException;
import java.time.OffsetDateTime;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.config.CookieSpecs;
import org.apache.http.client.config.RequestConfig;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.utils.URIBuilder;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.util.EntityUtils;
import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.ProducerRecord;
import org.json.JSONArray;
import org.json.JSONObject;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonDeserializer;
import com.google.gson.JsonObject;
import com.twitter.clientlib.model.Tweet;

import tweetoscope.utils.TweetMapper;

/**
 * Connects to the Twitter search stream endpoint and 
 * search for Tweets with given keywords (minimum 1, maximum 5).
 * As Tweets are received, Tweet subscribers are notified.
 * 
 * Requires the BEARER_TOKEN environment variable to be set 
 * to connect to the Twitter API endpoint.
 * 
 * To ensure only one instance of this class is created at any time, it
 * implements the Singleton pattern.
 * 
 * @author Sélim Ollivier, Maxime Raillat, Jérémie Levi
 *
 */
public final class TwitterFilteredStreamReaderSingleton extends OnlineTweetsProducer {

	/**
	 * Keywords to search for (minimum 1, maximum 5)
	 */
	protected List<String> keywords;

	/**
	 * Thread-safe implementation of the Singleton pattern
	 */
	private static volatile TwitterFilteredStreamReaderSingleton instance;

	/**
	 * The constructor declares 'private' access to implement the Singleton pattern
	 * 
	 * @param bootstrapServers servers where to fetch and send data (exemple: "localhost:9092").
	 * @param topicName name of the topic where to fetch extracted hashtags (exemple: Hashtags).
	 */
	private TwitterFilteredStreamReaderSingleton(String bootstrapServers, String topicName) {
        super(bootstrapServers, topicName);
		this.keywords = getKeywords();

		// run a couple of checks on the keywords (might not be enough though...)
		if (keywords.size() < 1  || keywords.size() >5 ) {
			System.out.println("You must provide between 1 and 5 keywords.");
			System.exit(1);
		}
		for (String keyword: keywords) {
			if (keyword.length() > 512) {
				System.out.println("The following rule contains " + keyword.length() + " characters "
						+ "while it must not exceed 243: \n");
				System.out.print(keyword);
				System.exit(1);
			}
		}

		try {
			Map<String, String> rules = new HashMap<String, String>();
			int i=0;
			for (String keyword: keywords) {
				rules.put(keyword, "rule " + i);
				i++;
			}
			setupRules(BEARER_TOKEN, rules);
			connectStream(BEARER_TOKEN);
		} catch (IOException | URISyntaxException e) {
			System.err.println(e);
			System.exit(-1);
		}
	}

	/**
	 * 
	 * @return single instance of the filtered Tweets producer
	 */
	public static TwitterFilteredStreamReaderSingleton getInstance(List<String> keywords) {
		// double-checked locking (DCL)
		// for a thread-safe implementation of the Singleton pattern
		// read more about DLC: https://refactoring.guru/java-dcl-issue
		TwitterFilteredStreamReaderSingleton result = instance;
		if (result != null) {
			return result;
		}
		synchronized (TwitterFilteredStreamReaderSingleton.class) {
			if (instance == null) {
				//instance = new TwitterFilteredStreamReaderSingleton(keywords);
			}
			return instance;
		}
	}

	/**
	 * Main mathod.
	 * 
	 * @param args command line arguments
	 **/
	public static void main(String[] args) {
		new TwitterFilteredStreamReaderSingleton(args[0], args[1]);
	}

	/**
	 * This method calls the search stream endpoint and reads Tweets from it.
	 * Requested Tweet fields: text and id, author_id, conversation_id, created_at, geo, lang
	 * Read more about the Tweet object model: 
	 * https://developer.twitter.com/en/docs/twitter-api/data-dictionary/object-model/tweet
	 * 
	 * @param bearerToken Twitter API access token
	 * @throws IOException
	 * @throws URISyntaxException
	 */
	private void connectStream(String bearerToken) throws IOException, URISyntaxException {

		HttpClient httpClient = HttpClients.custom()
				.setDefaultRequestConfig(RequestConfig.custom().setCookieSpec(CookieSpecs.STANDARD).build()).build();

		//URIBuilder uriBuilder = new URIBuilder("https://api.twitter.com/2/tweets/search/stream");
		/*
		URIBuilder uriBuilder = new URIBuilder(
				"https://api.twitter.com/2/tweets/search/stream?"
						+ "tweet.fields=author_id,conversation_id,created_at,geo,lang"
						+ "&expansions=geo.place_id&place.fields=contained_within,country,country_code,full_name,geo,id,name,place_type");
		 */ 
		URIBuilder uriBuilder = new URIBuilder(
				"https://api.twitter.com/2/tweets/search/stream?"
						+ "tweet.fields=author_id,conversation_id,created_at,geo,lang");

		HttpGet httpGet = new HttpGet(uriBuilder.build());
		httpGet.setHeader("Authorization", String.format("Bearer %s", bearerToken));

		HttpResponse response = httpClient.execute(httpGet);
		HttpEntity entity = response.getEntity();
		if (entity != null) {
			// TypeAdapter required to deserialize OffsetDateTime
			// see https://stackoverflow.com/questions/60723739/offsetdatetime-deserialization-using-gson
			Gson gson = new GsonBuilder()
					.registerTypeAdapter(OffsetDateTime.class, (JsonDeserializer<OffsetDateTime>) (json, type,
							context) -> OffsetDateTime.parse(json.getAsString()))
					.create();

			Tweet tweet;

			kafkaProducer = new KafkaProducer<Void, TweetMapper>(configureKafkaProducer());

			BufferedReader reader = new BufferedReader(new InputStreamReader((entity.getContent())));
			String line = reader.readLine();
			int nbTweets = 0;
			long startTime = System.currentTimeMillis();
			long observingPeriod = 60000;
			while (line != null) {
				if (System.currentTimeMillis() - startTime >= observingPeriod) {
					System.out.println("Filtered stream: " + nbTweets + " tweets received in " + observingPeriod + " milliseconds.");
					nbTweets = 0;
					startTime = System.currentTimeMillis();
				}
				if (!line.equals("")) {
					if (line.equals("Rate limit exceeded")) {
						System.out.println("Twitter search stream rate limit exceeded (25 requests per 15-minute window, "
								+ "see https://developer.twitter.com/en/docs/twitter-api/rate-limits). "
								+ "Try again in 15 minutes.");	
						System.exit(0);
					}
					if (line.contains("UsageCapExceeded")) {
						System.out.println("Twitter filtered search stream monthly usage cap (limit of 500,000 Tweets per month) exceeded. "
								+ "Try with another Twitter dev account or wait until next month...");
						System.exit(0);
					}
					tweet = gson.fromJson(gson.fromJson(line, JsonObject.class).get("data"), Tweet.class);
					kafkaProducer.send(new ProducerRecord<Void,TweetMapper>(topicName, new TweetMapper(tweet)));
					
					nbTweets++;
				}
				line = reader.readLine();
			}
		}
	}

	/**
	 * Helper method to setup rules before streaming data
	 * source: https://github.com/twitterdev/Twitter-API-v2-sample-code/blob/main/Filtered-Stream/FilteredStreamDemo.java
	 */
	private static void setupRules(String bearerToken, Map<String, String> rules) throws IOException, URISyntaxException {
		List<String> existingRules = getRules(bearerToken);
		if (existingRules.size() > 0) {
			deleteRules(bearerToken, existingRules);
		}
		createRules(bearerToken, rules);
	}

	/**
	 * Helper method to create rules for filtering
	 * source: https://github.com/twitterdev/Twitter-API-v2-sample-code/blob/main/Filtered-Stream/FilteredStreamDemo.java
	 * 
	 * @param bearerToken Twitter API access token
	 * @param rules rules to use for filtering
	 * 
	 * @throws IOException
	 * @throws URISyntaxException
	 */
	private static void createRules(String bearerToken, Map<String, String> rules) throws URISyntaxException, IOException {
		HttpClient httpClient = HttpClients.custom()
				.setDefaultRequestConfig(RequestConfig.custom()
						.setCookieSpec(CookieSpecs.STANDARD).build())
				.build();

		URIBuilder uriBuilder = new URIBuilder("https://api.twitter.com/2/tweets/search/stream/rules");

		HttpPost httpPost = new HttpPost(uriBuilder.build());
		httpPost.setHeader("Authorization", String.format("Bearer %s", bearerToken));
		httpPost.setHeader("content-type", "application/json");
		StringEntity body = new StringEntity(getFormattedString("{\"add\": [%s]}", rules));
		httpPost.setEntity(body);
		HttpResponse response = httpClient.execute(httpPost);

		HttpEntity entity = response.getEntity();
		if (null != entity) {
			String responseMessage = EntityUtils.toString(entity, "UTF-8");
			if (responseMessage.contains("error")) {
				System.out.println(EntityUtils.toString(entity, "UTF-8"));
			}
		}
	}

	/**
	 * Helper method to get existing rules
	 * source: https://github.com/twitterdev/Twitter-API-v2-sample-code/blob/main/Filtered-Stream/FilteredStreamDemo.java
	 */
	private static List<String> getRules(String bearerToken) throws URISyntaxException, IOException {
		List<String> rules = new ArrayList<>();
		HttpClient httpClient = HttpClients.custom()
				.setDefaultRequestConfig(RequestConfig.custom()
						.setCookieSpec(CookieSpecs.STANDARD).build())
				.build();

		URIBuilder uriBuilder = new URIBuilder("https://api.twitter.com/2/tweets/search/stream/rules");

		HttpGet httpGet = new HttpGet(uriBuilder.build());
		httpGet.setHeader("Authorization", String.format("Bearer %s", bearerToken));
		httpGet.setHeader("content-type", "application/json");
		HttpResponse response = httpClient.execute(httpGet);
		HttpEntity entity = response.getEntity();
		if (null != entity) {
			JSONObject json = new JSONObject(EntityUtils.toString(entity, "UTF-8"));
			if (json.length() > 1) {
				JSONArray array = (JSONArray) json.get("data");
				for (int i = 0; i < array.length(); i++) {
					JSONObject jsonObject = (JSONObject) array.get(i);
					rules.add(jsonObject.getString("id"));
				}
			}
		}
		return rules;
	}

	/**
	 * Helper method to delete rules
	 * source: https://github.com/twitterdev/Twitter-API-v2-sample-code/blob/main/Filtered-Stream/FilteredStreamDemo.java
	 */
	private static void deleteRules(String bearerToken, List<String> existingRules) throws URISyntaxException, IOException {
		HttpClient httpClient = HttpClients.custom()
				.setDefaultRequestConfig(RequestConfig.custom()
						.setCookieSpec(CookieSpecs.STANDARD).build())
				.build();

		URIBuilder uriBuilder = new URIBuilder("https://api.twitter.com/2/tweets/search/stream/rules");

		HttpPost httpPost = new HttpPost(uriBuilder.build());
		httpPost.setHeader("Authorization", String.format("Bearer %s", bearerToken));
		httpPost.setHeader("content-type", "application/json");
		StringEntity body = new StringEntity(getFormattedString("{ \"delete\": { \"ids\": [%s]}}", existingRules));
		httpPost.setEntity(body);
		httpClient.execute(httpPost);

		/*
		HttpEntity entity = response.getEntity();
		if (null != entity) {
			System.out.println(EntityUtils.toString(entity, "UTF-8"));
		}
		 */
	}

	/**
	 * source: https://github.com/twitterdev/Twitter-API-v2-sample-code/blob/main/Filtered-Stream/FilteredStreamDemo.java
	 */
	private static String getFormattedString(String string, List<String> ids) {
		StringBuilder sb = new StringBuilder();
		if (ids.size() == 1) {
			return String.format(string, "\"" + ids.get(0) + "\"");
		} else {
			for (String id : ids) {
				sb.append("\"" + id + "\"" + ",");
			}
			String result = sb.toString();
			return String.format(string, result.substring(0, result.length() - 1));
		}
	}

	/**
	 * source: https://github.com/twitterdev/Twitter-API-v2-sample-code/blob/main/Filtered-Stream/FilteredStreamDemo.java
	 */
	private static String getFormattedString(String string, Map<String, String> rules) {
		StringBuilder sb = new StringBuilder();
		if (rules.size() == 1) {
			String key = rules.keySet().iterator().next();
			return String.format(string, "{\"value\": \"" + key + "\", \"tag\": \"" + rules.get(key) + "\"}");
		} else {
			for (Map.Entry<String, String> entry : rules.entrySet()) {
				String value = entry.getKey();
				String tag = entry.getValue();
				sb.append("{\"value\": \"" + value + "\", \"tag\": \"" + tag + "\"}" + ",");
			}
			String result = sb.toString();
			return String.format(string, result.substring(0, result.length() - 1));
		}
	}

	private List<String> getKeywords() {
		List<String> keywords = new ArrayList<String>();
			keywords.add("Europe OR Africa OR Asia OR America OR Autralia OR Antartica "
					+ "OR Paris OR Berlin OR London OR Moscow OR Washington OR Beijing "
					+ "OR France OR Germany OR United Kingdom OR Russia OR USA OR China OR Brazil OR Israel "
					+ "OR Atlantic OR Pacific" + "OR place");

			keywords.add("climate OR sustainable OR energy OR free OR open OR peace OR war OR sport "
					+ "OR health OR Internet OR technology OR music OR award OR movie OR star OR pride "
					+ "OR food OR drink OR meal OR recipe OR routine OR economy OR business OR market OR science");

			keywords.add("Machine Learning OR data OR cloud OR social OR network OR woke OR election "
					+ "OR topic OR olympic OR game OR running OR trail OR workout OR challenge OR brand "
					+ "OR marketing OR advertisement OR news OR blockchain OR online OR store OR fashion");

			keywords.add("Monday OR Tuesday OR Wednesday OR Thursday OR Friday OR Saturday OR Sunday " + "OR 2022 "
					+ "OR time " + "OR week OR month OR year " + "OR yesterday OR today OR tomorrow "
					+ "OR last OR next OR soon " + "OR what OR who OR whom OR where OR when OR why");

			keywords.add("Apple OR Google OR Microsoft OR Amazon OR Facebook OR Coca-Cola OR Disney OR Samsung "
					+ "OR Intel OR NIKE OR Cisco 0R Oracle " + "OR Visa OR IBM OR Ikea " + "OR Netflix OR BMW "
					+ "OR Dion OR Thunberg " + "OR Messi OR Federer OR Djokovic " + "OR Rihanna OR Coldplay");
			
			return keywords;
	}
}
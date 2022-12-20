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

import java.awt.Color;
import java.awt.Dimension;
import java.time.Duration;
import java.util.Collections;
import java.util.Map;
import java.util.Properties;
import java.util.Map.Entry;
import java.util.stream.Stream;

import javax.swing.BorderFactory;
import javax.swing.JFrame;
import javax.swing.SwingUtilities;

import org.apache.kafka.clients.consumer.ConsumerConfig;
import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.apache.kafka.clients.consumer.ConsumerRecords;
import org.apache.kafka.clients.consumer.KafkaConsumer;
import org.apache.kafka.clients.producer.ProducerConfig;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.AxisLocation;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.data.category.DefaultCategoryDataset;

import tweetoscope.utils.LeaderBoardDeserializer;

/**
 * 
 * Displays the most popular hashtags as an histogram.
 * <p>
 * Leader board data is received via Java Flow (publisher =
 * {@link distributed_tweetoscope.HashtagCounter})
 * 
 * @author Sélim Ollivier, Maxime Raillat, Jérémie Levi
 *
 */
@SuppressWarnings("serial")
public class Visualizor extends JFrame {

	/**
	 * Dataset organized with a single row (key = {@code ROW_KEY}), and one column
	 * per hashtag. The column key is the hashtag text, the value stored at
	 * dataset(row:KEY_ROW, col: hashtag) is the number of occurrences of the
	 * hashtag.
	 */
	protected DefaultCategoryDataset dataset;
	/**
	 * Key of the single row of the {@code dataset} that contains the occurrences of
	 * hashtags
	 */
	protected final static String ROW_KEY = "hashtag";
	/*
	 * List of Kafka bootstrap servers. Example: localhost:9092,another.host:9092
	 * 
	 * @see:
	 * https://jaceklaskowski.gitbooks.io/apache-kafka/content/kafka-properties-
	 * bootstrap-servers.html
	 */
	protected String bootstrapServers;
	/*
	 * Name of the source Kafka topic
	 */
	protected String inputTopicName;
	/**
	 * Number of hashtags to include on the leader board
	 */
	protected int nbLeaders;
	/**
	 * Kafka consumer used to fetch Leader board updates
	 **/
	protected KafkaConsumer<Void,Map<String, Integer>> consumer;

	/**
	 * Main method.
	 * 
	 * @param args command line arguments.
	 **/
	public static void main(String[] args) {
		new Visualizor(args[0], args[1], Integer.valueOf(args[2]));
	}

	/**
	 * Constructor.
	 * 
	 * @param bootstrapServers servers where to fetch data (exemple: "localhost:9092").
	 * @param inputTopicName name of the topic where to fetch Leader board updates (exemple: Top).
	 * @param nbLeaders
	  */
	public Visualizor(String bootstrapServers, String inputTopicName, int nbLeaders) {
		this.bootstrapServers = bootstrapServers;
		this.inputTopicName = inputTopicName;
		this.nbLeaders = nbLeaders;

		initVisualizor();

		this.consumer = new KafkaConsumer<Void,Map<String, Integer>>(
			configureKafkaConsumer(bootstrapServers));
		// reads from any partition of the topic
		consumer.subscribe(Collections.singletonList(inputTopicName));

		try {
			Duration timeout = Duration.ofMillis(1000);
			while (true) {
				// reads events
				ConsumerRecords<Void,Map<String, Integer>> records = consumer.poll(timeout);
				for (ConsumerRecord<Void,Map<String, Integer>> record : records) {
					publish(record.value());
				}
			}
		} catch (Exception e) {
			System.out.println("something went wrong... " + e.getMessage());
		} finally {
			consumer.close();
		}
	}

	/**
	 * Sets up the graphical representation of the leader board.
	 */
	public void initVisualizor() {

		dataset = new DefaultCategoryDataset();

		JFreeChart chart = ChartFactory.createBarChart("Most Popular Hashtags", // title
				"", // category axis label
				"number of occurences", // value axis label
				dataset, // category dataset
				PlotOrientation.HORIZONTAL, // orientation
				false, // legend
				true, // tooltips
				false); // urls
		chart.getCategoryPlot().setRangeAxisLocation(AxisLocation.BOTTOM_OR_RIGHT);
		chart.getCategoryPlot().setDomainAxisLocation(AxisLocation.BOTTOM_OR_RIGHT);
		ChartPanel chartPanel = new ChartPanel(chart);
		chartPanel.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));
		chartPanel.setBackground(Color.white);
		chartPanel.setPreferredSize(new Dimension(500, 300));
		this.add(chartPanel);

		this.pack();
		this.setTitle("Tweetoscope");
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setVisible(true);
	}

	/**
	 * Triggered when the data to be displayed on the leader board changes:
	 * introduction of a new hashtag, or update of the number of occurrences of a
	 * hashtag already on the leader board. The new map is sent by the upstream
	 * component (HashtagCounter) via Kafka).
	 * 
	 * @param item New Leader board to display.
	 */
	public void publish(Map<String, Integer> item) {
		// setValue triggers the update of the graph
		// Swing and JFreeChart are not thread-safe, updates to the graph should be done
		// by the Event Dispaching Thread
		SwingUtilities.invokeLater(new Runnable() {

			@Override
			public void run() {
				// clears the leader board from previous data
				dataset.clear();
				// populates the leader board using data from the received map
				// sorted by reverse number of occurrences
				Stream<Entry<String, Integer>> sortedTopHashtags = item.entrySet().stream()
						.sorted(Collections.reverseOrder(Map.Entry.comparingByValue()));
				sortedTopHashtags.forEach(t -> {
					dataset.setValue(t.getValue(), ROW_KEY, t.getKey().toString());
				});
				// adds padding, if necessary (if we have not yet observed as many hashtags as
				// expected for the leader board
				for (int i = item.entrySet().size(); i < nbLeaders; i++) {
					dataset.setValue(0, ROW_KEY, "");
				}
			}
		});
	}

	/**
	 * Prepares configuration for the Kafka consumer <Void, Map<String,Integer>>.
	 * 
	 * @param bootstrapServers servers where to fetch data (exemple: "localhost:9092").
	 * 
	 * @return configuration properties for the Kafka consumer.
	 */
	protected Properties configureKafkaConsumer(String bootstrapServers) {
		Properties consumerProperties = new Properties();

		consumerProperties.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, bootstrapServers);
		consumerProperties.put(ConsumerConfig.KEY_DESERIALIZER_CLASS_CONFIG,
				org.apache.kafka.common.serialization.VoidDeserializer.class.getName());
		consumerProperties.put(ConsumerConfig.VALUE_DESERIALIZER_CLASS_CONFIG,
				LeaderBoardDeserializer.class);
		consumerProperties.put(ConsumerConfig.GROUP_ID_CONFIG, "Visualizor");
		consumerProperties.put(ConsumerConfig.AUTO_OFFSET_RESET_CONFIG, "earliest"); // from beginning

		return consumerProperties;
	}
}
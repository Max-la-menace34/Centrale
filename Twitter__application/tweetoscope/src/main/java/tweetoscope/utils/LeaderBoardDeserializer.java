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
package tweetoscope.utils;

import java.lang.reflect.Type;
import java.util.HashMap;

import org.apache.kafka.common.serialization.Deserializer;

import com.google.common.reflect.TypeToken;
import com.google.gson.Gson;

/**
 * Deserializes HashMap<String, Integer> to enable Kafka to exchange such type of data.
 * 
 * @author Sélim Ollivier, Maxime Raillat, Jérémie Levi
 **/
public class LeaderBoardDeserializer implements Deserializer<HashMap<String, Integer>> {
	/**
	 * Deserialize method.
	 * 
	 * @param topic
	 * @param data
	  */
    @Override
	public HashMap<String, Integer> deserialize(String topic, byte[] data) {
		if (data == null)
			return null;
		String jsonString = new String(data);
		Gson gson = new Gson();
        Type gsonType = new TypeToken<HashMap<String, Integer>>(){}.getType();
		return gson.fromJson(jsonString, gsonType);
	}
}
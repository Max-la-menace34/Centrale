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

import java.time.ZoneOffset;
import java.util.Date;

import com.twitter.clientlib.model.Tweet;

/**
 * Class used for serialize and deserialize recorded tweets with Kafka.
 * Some get/set methods are not accesible by Kafka for the class Tweet. Therefore, we need to create a custom class allowing us to exchange structured tweet data.
 * 
 * @author Sélim Ollivier, Maxime Raillat, Jérémie Levi
 */
public class TweetMapper {
    /** 
     * Tweet id
     **/
    private String id;
    /** 
     * author's account creation date.
     **/
    private Date created_at;
    /** 
     * Tweet's text
     **/
    private String text;
    /** 
     * Author id
     **/
    private String author_id;
    /** 
     * Conversation id
     **/
    private String conversation_id;
    /** 
     * Tweet language
     **/
    private String lang;

    /**
     * Default constructor
     **/
    public TweetMapper() {}

    /**
     * Parametrized constructor.
     * 
     * @param tweet
     **/
    public TweetMapper(Tweet tweet) {
        if (tweet != null) {
            setId(tweet.getId());
            try {
                setCreated_at(Date.from(tweet.getCreatedAt().toInstant()));
            } catch (NullPointerException e) {}

            setText(tweet.getText());
            setAuthor_id(tweet.getAuthorId());
            setConversation_id(tweet.getConversationId());
            setLang(tweet.getLang());
        }
    }

    public void setId(String id) {
        this.id = id;
    }

    public void setCreated_at(Date created_at) {
        this.created_at = created_at;
    }

    public void setText(String text) {
        this.text = text;
    }

    public void setAuthor_id(String author_id) {
        this.author_id = author_id;
    }

    public void setConversation_id(String conversation_id) {
        this.conversation_id = conversation_id;
    }

    public void setLang(String lang) {
        this.lang = lang;
    }

    public String getId() {
        return id;
    }

    public Date getCreated_at() {
        return created_at;
    }

    public String getText() {
        return text;
    }

    public String getAuthor_id() {
        return author_id;
    }

    public String getConversation_id() {
        return conversation_id;
    }

    public String getLang() {
        return lang;
    }

    /**
     * Converts TweetMapper object to Tweet object.
     * 
     * @return Tweet object.
      */
    public Tweet toTweet() {
        Tweet tweet = new Tweet();
        tweet.setId(this.getId());
        tweet.setCreatedAt(this.getCreated_at().toInstant().atOffset(ZoneOffset.UTC));
        tweet.setText(this.getText());
        tweet.setAuthorId(this.getAuthor_id());
        tweet.setConversationId(this.getConversation_id());
        tweet.setLang(this.getLang());

        return tweet;
    }

    /**
     * Converts TweetMapper[] array to Tweet array.
     * 
     * @param tweetMappers
     * 
     * @return Tweet[] array
      */
    protected Tweet[] toTweets(TweetMapper[] tweetMappers) {

        int nb_twwets = tweetMappers.length;
            Tweet[] tweets = new Tweet[nb_twwets];
            for (int i=0; i<nb_twwets; i++) {
                tweets[i] = new Tweet();
                tweets[i].setId(tweetMappers[i].getId());
                tweets[i].setCreatedAt(tweetMappers[i].getCreated_at().toInstant().atOffset(ZoneOffset.UTC));
                tweets[i].setText(tweetMappers[i].getText());
                tweets[i].setAuthorId(tweetMappers[i].getAuthor_id());
                tweets[i].setConversationId(tweetMappers[i].getConversation_id());
                tweets[i].setLang(tweetMappers[i].getLang());
            }
        return tweets;
    }
}



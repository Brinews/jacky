/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
//package com.izhfeng;

import java.util.*;
import java.text.DecimalFormat;

/**
 *
 * @author izhfeng
 */
// to store artists' names and albums' titles
public class Album {

    private String Aname;//name of artist
    private String Atitle;//title of album
    //to store tracks of an album
    public ArrayList<Track> TrackofAlbum;

    private static final DecimalFormat format = new DecimalFormat("00");

    Album() {
        this.Aname = "";
        this.Atitle = "";
        TrackofAlbum = new ArrayList<Track>();
    }

    Album(String aname, String atitle) {
        this.Aname = aname;
        this.Atitle = atitle;
        TrackofAlbum = new ArrayList<Track>();
    }

    public String getAname() {
        return Aname;
    }

    public String getAtitle() {
        return Atitle;
    }

    //name of artist and title of album
    public void setAname(String Aname) {
        this.Aname = Aname;
    }

    public void setTitle(String Title) {
        this.Atitle = Atitle;
    }

    public boolean findTrack(Track t) {
        for (int i = 0; i < TrackofAlbum.size(); i++) {
            if (TrackofAlbum.get(i).equals(t)) {
                return true;
            }
        }
        return false;
    }

    public boolean addTrack(Track t) {
        if (!findTrack(t)) {
            TrackofAlbum.add(t);
            return true;
        }
        return false;
    }

    public ArrayList<Track> getTrackList() {
        return TrackofAlbum;
    }

    //convert String duration to integer(seconds)
    public int getTotalDuration() {
        int total = 0;
        for (int i = 0; i < TrackofAlbum.size(); i++) {
            total += TrackofAlbum.get(i).getDuration().getTotalSeconds();
        }

        return total;
    }

    //convert integer(seconds) to String in order to print out
    public String getTotalDurationString() {

        String str = "";

        int hours, minutes, seconds;
        int total = getTotalDuration();

        if (total <= 0) {
            hours = 00;
            minutes = 00;
            seconds = 00;
            str = format.format(hours) + ":" + format.format(minutes) + ":" + format.format(seconds);
        } else {
            minutes = total / 60;
            if (minutes < 60) {
                seconds = total % 60;
                hours = 00;
                str = format.format(hours) + ":" + format.format(minutes) + ":" + format.format(seconds);
            } else {
                hours = minutes / 60;
                minutes = minutes % 60;
                seconds = total - 60 * (60 * hours + minutes);
                str = format.format(hours) + ":" + 
                    format.format(minutes) + ":" + format.format(seconds);
            }
        }

        return str;
    }

    public String toString() {
        String str = Aname + " : " + Atitle + "\n";
        for (int i = 0; i < TrackofAlbum.size(); i++) {
            str += TrackofAlbum.get(i).toString() + "\n";
        }
        return str;
    }

    public boolean equals(Album a) {
        return Aname.equals(a.Aname) && Atitle.equals(a.Atitle);
    }

    public Track getMaxDurationTrack() {
        Track maxTrack = null;
        int maxDuration = 0;

        for (int i = 0; i < TrackofAlbum.size(); i++) {
            if (TrackofAlbum.get(i).getDuration().getTotalSeconds() 
                    > maxDuration) {
                maxTrack = TrackofAlbum.get(i);
            }
        }

        return maxTrack;
    }
}

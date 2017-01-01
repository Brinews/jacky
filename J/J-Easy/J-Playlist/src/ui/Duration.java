/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
//package com.izhfeng;

import java.text.DecimalFormat;

/**
 *
 * @author izhfeng
 */
public class Duration {

    private int hours;
    private int minutes;
    public int seconds;

    private static final DecimalFormat format = new DecimalFormat("00");

    public Duration()
    {
        this.hours = 00;
        this.minutes = 00;
        this.seconds = 00;
    }

    public Duration(int h,int m,int s)
    {
        this.hours = h;
        this.minutes = m;
        this.seconds = s;
    }

    public Duration(String str)
    {
        setDuration(str);
    }

    public void setDuration(String str)
    {
        String[] token = str.split(":");

        hours = Integer.parseInt(token[0]);
        minutes = Integer.parseInt(token[1]);
        seconds = Integer.parseInt(token[2]); 
    }

    public int getHours() {
        return hours;
    }

    public void setHours(int hours) {
        this.hours = hours;
    }

    public int getMinutes() {
        return minutes;
    }

    public void setMinutes(int minutes) {
        this.minutes = minutes;
    }

    public int getSeconds() {
        return seconds;
    }

    public void setSeconds(int seconds) {
        this.seconds = seconds;
    }

    /*
     * time translate
     */
    public int getTotalSeconds()
    {
        return (hours*60+minutes)*60 + seconds;
    }

    public boolean equals(Duration d) {
        return hours == d.hours && minutes == d.minutes && seconds == d.seconds;
    }

    @Override
    public String toString()
    {
        return format.format(hours) + ":" + format.format(minutes) 
            + ":" + format.format(seconds);
    }
}

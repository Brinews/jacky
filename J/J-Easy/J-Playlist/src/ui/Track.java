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
public class Track {
    
    private String title;//title of Track    
    private Duration dura;
    
    Track() {
        this.title = "";
        dura = new Duration();
    }
    
    Track(String t, Duration d){
        this.title = t;
        dura = d;
    }
    
    public String getTitle() {
        return title;
    }

    public void setTitle(String t) {
        title = t;
    }

    public Duration getDuration() {
        return dura;
    }

    public void setDuration(Duration d) {
        dura = d;
    }

    public boolean equals(Track t) {
        return title.equals(t.title) && dura.equals(t.dura);
    }
       
    @Override
    public String toString()
    {
        return dura.toString() + " - " + title;
    }
}

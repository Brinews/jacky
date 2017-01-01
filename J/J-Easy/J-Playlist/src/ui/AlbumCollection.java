/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
//package com.izhfeng;

import java.util.List;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.Collections;

/**
 *
 * @author izhfeng
 */
public class AlbumCollection {

    ArrayList<Album> DetailsofAlbum;

    AlbumCollection() {
        DetailsofAlbum = new ArrayList<Album>();
    }

    public ArrayList<Album> getCollectionList() {
        return DetailsofAlbum;
    }

    public boolean findAlbum(Album a) {
        for (int i = 0; i < DetailsofAlbum.size(); i++) {
            if (DetailsofAlbum.get(i).equals(a)) {
                return true;
            }
        }
        return false;
    }

    public boolean addAlbum(Album a) {
        if (findAlbum(a) == false) {
            DetailsofAlbum.add(a);
            return true;
        }

        return false;
    }
    
    public Album getAlbum(int idx) {
        int cnt = 0;
        for (int i = 0; i < DetailsofAlbum.size(); i++) {
            if (idx >= cnt && 
                    idx < cnt + DetailsofAlbum.get(i).getTrackList().size()) {
                return DetailsofAlbum.get(i);
            }

            cnt += DetailsofAlbum.get(i).getTrackList().size();
        }

        return null;
    }

    //to find out which one has largest number of tracks
    public Album getMostTracksAlbum() {
        int Asize = DetailsofAlbum.size();
        Album ret = null;
        int maxCnt = 0;

        for (int i = 0; i < Asize; i++) {
            int cnt = DetailsofAlbum.get(i).TrackofAlbum.size();
            if (cnt > maxCnt) {
                maxCnt = cnt;
                ret = DetailsofAlbum.get(i);
            }
        }

        return ret;
    }
    
    //to find out the album with longest track
    public Album getLongestTrackAlbum() {
        Track maxTrack = null;
        Album ret = null;

        for (int i = 0; i < DetailsofAlbum.size(); i++) {
            Track track = DetailsofAlbum.get(i).getMaxDurationTrack();
            if (track != null) {
                if (maxTrack == null) maxTrack = track;
                else if (track.getDuration().getTotalSeconds() > 
                        maxTrack.getDuration().getTotalSeconds()) {
                    maxTrack = track;
                    ret = DetailsofAlbum.get(i);
                }
            }
        }

        return ret;
    }

    public String toString() {
        String ret = "";
        for (int i = 0; i < DetailsofAlbum.size(); i++) {
            ret += DetailsofAlbum.get(i).toString();
        }
        return ret;
    }
}

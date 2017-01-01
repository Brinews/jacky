/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
//package com.izhfeng;

import java.util.*;

/**
 *
 * @author izhfeng
 */
public class PlaylistTrack extends Track {

    private Album refAlbum;

    PlaylistTrack() {
        super();
        refAlbum = null;
    }

    PlaylistTrack(String t, Duration d) {
        super(t, d);
        refAlbum = null;
    }

    PlaylistTrack(String t, Duration d, Album a) {
        super(t, d);
        refAlbum = a;
    }

    public Album getRefAlbum() {
        return refAlbum;
    }

    public String getAlbumArtist() {
        return refAlbum.getAname();
    }

    public String getAlbumTitle() {
        return refAlbum.getAtitle();
    }

    public void setRefAlbum(AlbumCollection ac) {

        if (ac != null) {
            ArrayList<Album> al = ac.getCollectionList();
            for (int i = 0; i < al.size(); i++) {
                if (al.get(i).findTrack(new Track(getTitle(), getDuration()))) {
                    refAlbum = al.get(i); // find
                    break;
                }
            }
        }
    }

    public boolean equals(PlaylistTrack t) {
        return super.equals(t) && refAlbum == t.refAlbum;
    }
}

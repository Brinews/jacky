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
public class Playlist {

    public ArrayList<PlaylistTrack> Pal;
    AlbumCollection acl;

    Playlist(AlbumCollection ac){
        Pal = new ArrayList<PlaylistTrack>();
        acl = ac;
    }

    public PlaylistTrack findTrackInPlaylist(PlaylistTrack t) {
        for (int i = 0; i < Pal.size(); i++) {
            if (Pal.get(i).equals(t)) return Pal.get(i);
        }
        return null;
    }

    public Album getAlbum(int i) {
        if (i >= 0 && i < Pal.size()) {
            return Pal.get(i).getRefAlbum();
        }
        return null;
    }

    public boolean addTrackToPlaylist(String title, String duration) {

        Duration d = new Duration(duration);
        PlaylistTrack plt = new PlaylistTrack(title, d);
        plt.setRefAlbum(acl);

        PlaylistTrack found = findTrackInPlaylist(plt);

        if (null == found) {
            Pal.add(plt);
            return true;
        } else {
            found.setRefAlbum(acl);
        }

        return false;
    }

    public boolean removeTrack(int id) {
        if (id >= 0 && id < Pal.size()) {
            Pal.remove(id);
            return true;
        }
        return false;
    }

    public ArrayList<PlaylistTrack> getTrackList() {
        return Pal;
    }

    public int getTotalDuration(){
        int time = 0;
        for(int i=0;i<Pal.size();i++){
            time += Pal.get(i).getDuration().getTotalSeconds();
        }

        return time;
    }

    public String toString() {
        String ret = "";
        for (int i = 0; i < Pal.size(); i++) {
            ret += Pal.get(i).toString() + "\n";
        }
        return ret;
    }
}

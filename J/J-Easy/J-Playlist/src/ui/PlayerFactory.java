/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
//package com.izhfeng;

import java.util.*;
import java.io.*;
import java.text.DecimalFormat;

/**
 *
 * @author izhfeng
 */
public class PlayerFactory {

    //implementation of reading txt 
    public static void loadAlbums(AlbumCollection ac, File file) {
        try {
            String encoding = "UTF-8";
            if (file.isFile() && file.exists()) {
                InputStreamReader read = new InputStreamReader(
                        new FileInputStream(file), encoding);

                BufferedReader bufferedReader = new BufferedReader(read);

                String lineTxt = null;
                Album a = null;

                while ((lineTxt = bufferedReader.readLine()) != null) {
                    /**
                     * make sure if string has "-" if it has, then the string
                     * contains duration and title of track otherwise it
                     * contains name of artist and title of album
                     */
                    if (lineTxt.indexOf("-") != -1) { // add tracks
                        String[] items = lineTxt.split(" - ");
                        Duration d = new Duration(items[0]);
                        Track t = new Track(items[1], d);
                        a.addTrack(t);
                    } else{ // add album
                        String[] items = lineTxt.split(" : ");
                        a = new Album(items[0], items[1]);
                        ac.addAlbum(a);
                    }
                }
            }
        } catch (Exception e) {
            System.out.println("scan file error");
            e.printStackTrace();
        }
    }

    public static void saveAlbums(AlbumCollection ac, File file) {
        try {
            BufferedWriter bufw = new BufferedWriter(new FileWriter(file));
            bufw.write(ac.toString());
            bufw.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void loadPlaylist(Playlist plt, File file) {
        try {
            String encoding = "UTF-8";
            if (file.isFile() && file.exists()) {
                InputStreamReader read = new InputStreamReader(
                        new FileInputStream(file), encoding);

                BufferedReader bufferedReader = new BufferedReader(read);

                String lineTxt = null;
                while ((lineTxt = bufferedReader.readLine()) != null) {
                    /**
                     * each line contains duration and title of track 
                     */
                    if (lineTxt.indexOf("-") != -1) { // add tracks
                        String[] items = lineTxt.split(" - ");
                        plt.addTrackToPlaylist(items[1], items[0]);
                    }
                }
            }
        } catch (Exception e) {
            System.out.println("scan file error");
            e.printStackTrace();
        }
    }

    public static void savePlaylist(Playlist plt, File file) {
        try {
            BufferedWriter bufw = new BufferedWriter(new FileWriter(file));
            bufw.write(plt.toString());
            bufw.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static ArrayList<String[]> getAlbumsTrack(AlbumCollection acl) {
        ArrayList<String[]> ret = new ArrayList<String[]>();
        
        ArrayList<Album> albums = acl.getCollectionList();
        for (int i = 0;i < albums.size(); i++) {
            Album a = albums.get(i);
            ArrayList<Track> tracks = a.getTrackList();
            for (int j = 0; j < tracks.size(); j++) {
                String[] item = new String[3];
                item[0] = a.getAtitle();
                item[1] = tracks.get(j).getTitle();
                item[2] = tracks.get(j).getDuration().toString();

                ret.add(item);
            }
        }

        return ret;
    }

    public static ArrayList<String[]> getPlaylistTrack(Playlist plt) {
        ArrayList<String[]> ret = new ArrayList<String[]>();

        ArrayList<PlaylistTrack> tracks = plt.getTrackList();
        for (int i = 0; i < tracks.size(); i++) {
            String[] item = new String[3];
            if (tracks.get(i).getRefAlbum() != null) {
                item[0] = tracks.get(i).getRefAlbum().getAtitle();
            } else {
                item[0] = "";
            }
            item[1] = tracks.get(i).getTitle();
            item[2] = tracks.get(i).getDuration().toString();

            ret.add(item);
        }

        return ret;
    }
}

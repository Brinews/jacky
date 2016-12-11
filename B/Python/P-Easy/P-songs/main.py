# main test

#
# file format
# Author: xxx Album: xxx Songs: aaa bbb ccc
#

from artist import *
import os

def LoadFile(filename):
    plist = Playlist()
    try:
        infile = open(filename, "r")
        lines = infile.read()
        infile.close()

        info = lines.split("\n")

        for item in info:
            if len(item) < 1:
                break

            items = item.split("\t")
            artist = str(items[1])
            album = Album(items[3])

            i = 5
            while i < len(items):
                song = Song(items[i])
                album.addSong(song)
                i = i + 1

            plist.addPlayList(artist, album)

    except IOError:
        print("Error: File open error\n")

    return plist

def SaveFile(playlist, filename):
    try:
        outfile = open(filename, 'w')
        outfile.write(str(playlist))
        outfile.close()
    except IOError:
        print("Error: File writing error")

def Menu():
    print("1. Add artist and album\n")
    print("2. Add songs to album\n")
    print("3. List all artists\n")
    print("4. List songs of given artist\n")
    print("5. Quit\n")

def Main():
    playlist = LoadFile("in.txt")
    while (True):
        Menu()
        inp = str(input("Options >> ")).strip()

        if inp == "1":
            artist1 = str(raw_input("Enter artist name:")).strip()
            title1 = str(raw_input("Enter album title:")).strip()
            album = Album(title1)
            playlist.addPlayList(artist1, album)
            print("Add Complete.\n")
        elif inp == "2":
            artist2 = str(raw_input("Enter artist name:")).strip()
            title2 = str(raw_input("Enter album title:")).strip()
            alb = playlist.findAlbums(artist2, title2) 
            if alb != None:
                songtitle = str(raw_input("Enter Songs Name:")).strip()
                if alb.getSong(songtitle) == None:
                    alb.addSong(Song(songtitle))
                    print("Add Song Success!\n")
                else:
                    print("Song Already Exist.\n")
            else:
                 print("No Album Found.\n")
        elif inp == "3":
            playlist.listArtists()
        elif inp == "4":
            print(playlist)
        elif inp == "5":
            break

    SaveFile(playlist, "in.txt")

# call test
Main()

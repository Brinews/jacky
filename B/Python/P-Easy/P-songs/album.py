#
# Album: Song List
# Date: 2016-12-10
#

from song import *

class Album:
    def __init__(self, title):
        self._title = title
        self._songslist = []

    def getTitle(self):
        return self._title

    def setTitle(self, title):
        self._title = title

    def getSongs(self):
        return self._songslist

    def getSong(self, songtitle):
        for i in range(len(self._songslist)):
            if self._songslist[i].getTitle() == songtitle:
                return self._songslist[i]
        return None

    def addSong(self, song):
        self._songslist.append(song)

    def __str__(self):
        string = "Album:\t'" + self._title + "'\tSongs:\t"
        for songs in self._songslist:
            string += str(songs)
            string += "\t"

        return string

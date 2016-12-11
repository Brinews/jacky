#
# Date: 2016-12-10
# Brief Description: artist's album
#

from album import *

class Playlist:
    def __init__(self):
        self._playdict = {}

    def addPlayList(self, artist, album):
        if self._playdict.has_key(artist):
            found = False
            albs = self.getAlbums(artist)
            for alb in albs:
                if alb.getTitle() == album.getTitle():
                    found = True
            if found == False:
                self._playdict[artist].append(album)
        else:
            self._playdict[artist] = [album];

    def findAlbums(self, artist, title):
        if self._playdict.has_key(artist):
            alb = self.getAlbums(artist)
            for a in alb:
                if a.getTitle() == title:
                    return a

        return None

    def getAlbums(self, artist):
        if self._playdict.has_key(artist):
            return self._playdict[artist]
        return None

    def listArtists(self):
        print("Artist List\n_______________")
        for k in self._playdict.keys():
            print("'"+k+"'")

    def listAlbums(self):
        for artist in self._playdict.keys():
            alb = self.getAlbums(artist)
            if alb != None:
                for a in alb:
                    print(a)

    def __str__(self):
        string = ""
        for k in self._playdict.keys():
            alb = self.getAlbums(k)
            for a in alb:
                string += "Artist:\t'" + str(k) + "'"
                string += "\t"
                string += str(a)
                string += "\n"

        return string

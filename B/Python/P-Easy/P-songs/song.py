#
# Song class
# Date: 2016-12-10
#

class Song:
    def __init__(self, title):
        self._title = title

    def getTitle(self):
        return self._title

    def setTitle(self, title):
        self._title = title

    def __str__(self):
        return "'{0}'".format(self._title)


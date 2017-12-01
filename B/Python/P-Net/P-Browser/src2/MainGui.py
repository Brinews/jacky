#coding=utf-8  
from tkinter import *  
import tkinter as tk

from main import start_search
from main import worker
from OrderedList import OrderedList
from sqlite_engine import connDB
  
class WidgetsStart:  
    def __init__(self):  
        # db init
        self.keycache = OrderedList()
        self.db = connDB()

        window = tk.Tk()             
        window.title("关键词搜索")
          
        frame1 = Frame(window)   
        frame1.pack()   

        header = Message(frame1, text="检索")  
        header.grid(row=1, column=3)  

        frame2 = Frame(window)
        frame2.pack()        

        self.v2 = IntVar() 
        rbRed = Radiobutton(frame2,text="显示搜索结果链接",bg="red",  
                            variable=self.v2,value=1,  
                            command=self.processRadiobutton)  
        rbYellow = Radiobutton(frame2,text="显示关联关键词",bg="yellow",  
                               variable=self.v2,value=2,  
                               command=self.processRadiobutton)  

        label = Label(frame2, text="Enter the keyword: ") 
        self.name = StringVar()  

        entryName = Entry(frame2, textvariable=self.name)  
        btGetName = Button(frame2, text="Search",  
                           command=self.processButton)  
          
        # grid layout
        label.grid(row=1,column=1)  
        entryName.grid(row=1,column=2)  
        btGetName.grid(row=1,column=3)  
          
        # grid
        rbRed.grid(row=2,column=2)  
        rbYellow.grid(row=2,column=3)  

        # display text
        self.text = Text(window)  
        self.text.pack()  
          
        window.mainloop()  
      
    # actions
    def processRadiobutton(self):  

        self.text.delete(1.0, END)

        if self.v2.get() == 1:
            res = self.db.do_query(self.keys)
            for r in res:
                self.text.insert(END, "\t".join(r))
                self.text.insert(END, "\n\n")
        else:
            res = self.db.do_query_tag(self.keys)
            for r in res:
                self.text.insert(END, "\t".join(r))
                self.text.insert(END, "\n\n")
      
    def processButton(self):  
        #print ("Your name is " + self.name.get())  
        n = 20 # default links number

        self.text.delete(1.0, END)

        self.text.insert(END, "Searching...\n")
        worker(0, self.name.get(), n, self.keycache)
        self.keys = self.name.get()
        self.text.insert(END, "Completed!")
      
WidgetsStart()  


# -*- coding: utf-8 -*-  
from socket import * 
import sys 

if len(sys.argv) <= 1: 
	print 'Usage : "python ProxyServer.py server_ip"\n[server_ip : It is the IP Address Of Proxy Server' 
	sys.exit(2) 

# Create a serversocket, bind it to a port and start listening 
tcpSerSock = socket(AF_INET, SOCK_STREAM) 
# Fill in start. 
tcpProxyPort = 8888 					#指定端口, 默认为8888, 可以修改
tcpSerSock.bind(("", tcpProxyPort))		#进行IP地址及端口绑定
tcpSerSock.listen(6)					#开始监听, 指定可重复次数6, 可修改
# Fill in end. 
while 1: 
	# Strat receivingdata from the client 
	print 'Ready to serve...' 
	tcpCliSock, addr = tcpSerSock.accept() #等待客户端接入
	print 'Received aconnection from:', addr 
	message = tcpCliSock.recv(1024)		   #接收客户端发送数据, 数据空间长度为1024 # Fill in start.  # Fill in end.
	print message 
	# Extract the filename from the given message 
	print message.split()[1] 
	filename = message.split()[1].partition("/")[2] #取出数据中文件名字段
	print filename 
	fileExist = "false" 
	filetouse = "/" +filename 
	print filetouse 
	try: 
		# Check wether the file exist in the cache 
		f = open(filetouse[1:], "r") 
		outputdata = f.readlines() 
		fileExist = "true" 
		# ProxyServer finds a cache hit and generates a response message 
		tcpCliSock.send("HTTP/1.0 200 OK\r\n") 
		tcpCliSock.send("Content-Type:text/html\r\n") 
		# Fill in start. 
		for i in range(0, len(outputdata)): #发送缓存数据到请求客户,相当于代理
			tcpCliSock.send(outputdata[i])
		# Fill in end.
		print 'Read from cache' 
	# Error handling for file not found in cache 
	except IOError: 
		if fileExist == "false": 
			# Create a socketon the proxyserver 
			c = socket(AF_INET, SOCK_STREAM) #建立一个网络连接, 准备从远程服务器取客户需要的数据 # Fill in start.  # Fill in end.
			hostn = filename.replace("www.","",1) 
			print hostn  					 #将网页名称对应为主机地址
			try: 
				# Connect to the socket to port 80
				# Fill in start. 
				c.connect(hostn, 80) #连接远端服务器的80端口
				# Fill in end.
				# Create a temporary file on this socket and ask port 80 for the file requested by the client 
				fileobj = c.makefile('r', 0) 
				fileobj.write("GET "+"http://" + filename + "HTTP/1.0\n\n") 
				# Read the response into buffer 
				# Fill in start. 
				proxyBuf = fileobj.readlines() #读取远端文件到本地
				# Fill in end.
				# Create a new file in the cache for the requested file. 
				#  Also  send  the response  in  the buffer to client socket and the corresponding file in the cache 
				tmpFile = open("./" + filename,"wb") 

				# Fill in start. 
				tcpCliSock.send("HTTP/1.0 200 OK\r\n") #发送HTTP头
				tcpCliSock.send("Content-Type:text/html\r\n") 
				for i in range(0, len(proxyBuf)): 
					tmpFile.write(proxyBuf[i])	     #保存到本地缓存文件
					tcpCliSock.send(proxyBuf[i]) #发送客户端请求文件
				# Fill in end.
			except: 
				print "Illegal request" 
		else: 
			# HTTP response message for file not found 
			# Fill in start. 
			tcpCliSock.send("404 Not found") #发送未找到消息
			# Fill in end. 
	# Close the client and the server sockets 
	tcpCliSock.close() 
# Fill in start. 
if __name__ == '__main__': #程序入口, 从这里开始执行
	main()
# Fill in end.


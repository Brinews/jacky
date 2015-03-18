package com.example.wiffi1;

import java.io.Serializable;

//必须实现Serializable接口。否则无法调用ObjectOutputStream的
//writeObject方法，或者ObjectInputStream中的readObject方法

public class FileSerIo implements Serializable
{
	private static final long serialVersionUID = 1L;

	private String fileName;            // 文件名称
	private long fileLength;             // 文件长度
	private byte[] fileContent;          // 文件内容

	public String getFileName()
	{
		return fileName;
	}

	public void setFileName(String fileName)
	{
		this.fileName = fileName;
	}

	public long getFileLength()
	{
		return fileLength;
	}

	public void setFileLength(long fileLength)
	{
		this.fileLength = fileLength;
	}

	public byte[] getFileContent()
	{
		return fileContent;
	}

	public void setFileContent(byte[] fileContent)
	{
		this.fileContent = fileContent;
	}
}

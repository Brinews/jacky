package disk;

import global.DatabaseConstants;

import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;

/**
 * Page
 */
public class Page implements DatabaseConstants {

	// Stores the page's actual data
	protected byte[] data;

	/**
	 * Creates a new blank page
	 */
	public Page() {
		data = new byte[PAGE_SIZE];
	}

	/**
	 * Creates a new page using the given byte array
	 * @param data
	 */
	public Page(byte[] data) {
		setData(data);
	}

	/**
	 * Setter for the data byte array
	 * @param data
	 */
	public void setData(byte[] data) {
		assert(data.length == PAGE_SIZE);
		this.data = data;
	}

	/**
	 * Getter for the data byte array
	 */
	public byte[] getData() {
		return this.data;
	}

	/**
	 * Copy the contents of one page into this object
	 * @param from - the page to copy from
	 */
	public void copy(Page from) {
		System.arraycopy(from.data, 0, this.data, 0, PAGE_SIZE);
	}

	/**
	 * Get the boolean value at the offset. Note: we are storing a boolean using 1 byte.
	 * This is less than ideal but sufficient for our purpose
	 * @param offset position of the boolean in the byte array
	 * @return false if data[offset] is 1, else true
	 */
	public boolean getBooleanValue(int offset) {
		return data[offset] != 0;
	}

	/**
	 * Sets the boolean value at the offset
	 * @param value boolean value to set
	 * @param offset position of the boolean in the byte array
	 */
	public void setBooleanValue(boolean value, int offset) {
		data[offset] = value ? (byte) 1 : (byte) 0;
	}

	/**
	 * Gets the string with the given length at the offset
	 * @param offset - position of the double in the byte array
	 * @return the string in the data array
	 */
	public String getStringValue(int offset) {
		short length = ByteBuffer.wrap(data, offset, 2).getShort();
		return new String(data, offset+2, length, StandardCharsets.US_ASCII);
	}

	/**
	 * Puts the string contents at the offset using US_ASCII charset.
	 * It requires 2 + value.length bytes, and assumes the string will not be longer
	 * that Short.MAX_VALUE
	 * @param value string to put in the array
	 * @param offset start position of the string in the array
	 */
	public void setStringValue(String value, int offset) {
		ByteBuffer.wrap(data, offset, 2 + value.length())
				.putShort((short) value.length())
				.put(value.getBytes(StandardCharsets.US_ASCII), 0, value.length());
	}

	/**
	 * Gets the double value at the offset (takes up 8 bytes)
	 * @param offset position of the double in the byte array
	 * @return the double value
	 */
	public double getDoubleValue(int offset) {
		return ByteBuffer.wrap(data, offset, 8).getDouble();
	}

	/**
	 * Sets the double value at the offset
	 * @param value double value to set
	 * @param offset start position of the double in the array
	 */
	public void setDoubleValue(double value, int offset) {
		ByteBuffer.wrap(data, offset, 8).putDouble(value);
	}

	/**
	 * Gets the integer value at the offset (takes up 4 bytes)
	 * @param offset start position of the integer in the byte array
	 * @return the integer value
	 */
	public int getIntegerValue(int offset) {
		return ByteBuffer.wrap(data, offset, 4).getInt();
	}

	/**
	 * Sets the integer value at the offset
	 * @param value integer value to set
	 * @param offset start position of the integer in the array
	 */
	public void setIntegerValue(int value, int offset) {
		ByteBuffer.wrap(data, offset, 4).putInt(value);
	}

}

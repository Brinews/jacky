package buffer.policy;

import buffer.BufferFrame;

import java.util.List;

/**
 * Interface for Buffer Replacement Strategy
 */
public interface ReplacementPolicy {

	/**
	 * Returns a unique string for the replacement policy
	 */
	public String getName();

	/**
	 * Chooses the BufferFrame to replace
	 * @param pool current buffer pool
	 * @return chosen buffer frame for replacement
	 */
	public BufferFrame choose(List<BufferFrame> pool) throws BufferFrame.AllBufferFramesPinnedException;

	/**
	 * Called whenever a frame is accessed.
	 * @param pool current buffer pool
	 * @param frame the frame that was accessed
	 */
	public void notify(List<BufferFrame> pool, BufferFrame frame);

}
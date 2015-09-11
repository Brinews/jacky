package buffer.policy;

import buffer.BufferFrame;

import java.util.List;

/**
 * GLOCK Replacement Policy
 */
public class GeneralisedClockReplacementPolicy implements ReplacementPolicy {

    private final int mLimit;
    private int mCurrent;

    public GeneralisedClockReplacementPolicy(int limit) {
        mLimit = limit;
        mCurrent = 0;
    }

    @Override
    public String getName() {
        return "GCLOCK";
    }

    @Override
    public BufferFrame choose(List<BufferFrame> pool) {
        assert pool.size() > 0 : "Expects a pool of at least size 1";
        // Check to see if every page is pinned
        boolean allPinned = true;
        for(BufferFrame frame : pool) {
            allPinned = allPinned && frame.isPinned();
        }
        if(allPinned) throw new BufferFrame.AllBufferFramesPinnedException();
        // Move Clock Hand and Decrement Values until we find a frame with a zero usage count
        BufferFrame temp;
        for(int i = mCurrent; true; i = (i + 1) % pool.size()) {
            temp = pool.get(i);
            if(temp.getClockCount() == 0 && !temp.isPinned()) {
                mCurrent = (i + 1) % pool.size();
                return temp;
            }
            else temp.setClockCount(Math.max(temp.getClockCount() - 1, 0));
        }
    }

    @Override
    public void notify(List<BufferFrame> pool, BufferFrame frame) {
        int max = Math.min(frame.getClockCount() + 1, mLimit);
        frame.setClockCount(max);
    }

}

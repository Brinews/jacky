package buffer;

import buffer.policy.ReplacementPolicy;
import disk.Page;
import disk.PageId;
import global.DBComponent;
import global.DatabaseConstants;

import java.io.IOException;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

/**
 * Buffer Manager for the database
 */
public class BufferManager implements DatabaseConstants {

    private ReplacementPolicy mReplacementPolicy;
    private List<BufferFrame> mBufferFrames;
    private int cacheHits;
    private LinkedList<Integer> pageAccesses;

    public BufferManager(ReplacementPolicy replacementPolicy) {
        mReplacementPolicy = replacementPolicy;
        mBufferFrames = new LinkedList<>();
        cacheHits = 0;
        pageAccesses = new LinkedList<>();
    }

    /**
     * Obtains a page from the buffer.
     * If the page is not in the buffer, it will fetch it from disk
     * @param pageId the id of the page to fetch
     * @return the requested page
     */
    public Page getPage(PageId pageId) {
        pageAccesses.add(pageId.get()); // Logging of page accesses
        BufferFrame found = findFrameByPageId(pageId);
        // If page is in the cache (cache hit)
        if(found != null) {
            mReplacementPolicy.notify(mBufferFrames, found);
            cacheHits++;
            return found.getPage();
        }
        // Get replacement frame
        BufferFrame frame = getBufferFrame();
        replaceFrameInBuffer(pageId, frame);
        return frame.getPage();
    }

    /**
     * Replaces a given BufferFrame with a new page and page id
     * @param toReplace the id of the page to insert into the frame
     * @param current the frame to be changed
     */
    protected void replaceFrameInBuffer(PageId toReplace, BufferFrame current) {
        if(current.isDirty()) {
            flushPage(current);
        }
        Page replacement = new Page();
        try {
            // Read page in from disk and add it to the buffer frame
            DBComponent.DISK_MANAGER().readPage(toReplace, replacement);
            current.setPage(toReplace, replacement);
        }
        catch (BufferFrame.BufferFrameDirtyException | IOException e) {}
        mReplacementPolicy.notify(mBufferFrames, current);
    }

    /**
     * Flushes all dirty frames in the buffer manager
     * @return number of frames flushed to disk
     */
    public int flushDirty() {
        int flushCount = 0;
        for(BufferFrame frame : mBufferFrames) {
            if(frame.isDirty()) {
                flushPage(frame);
                flushCount++;
            }
        }
        return flushCount;
    }

    /**
     * Writes the page to disk
     * @param frame containing the page to write to disk
     */
    public void flushPage(BufferFrame frame) {
        try {
            DBComponent.DISK_MANAGER().writePage(frame.getPageId(), frame.getPage());
            frame.unpin();
            frame.setDirty(false);
        }
        catch(IOException e) {}
    }

    /**
     * Creates a new page on disk
     * @return id of the new page
     */
    public PageId getNewPage() {
        return DBComponent.DISK_MANAGER().allocatePage();
    }

    /**
     * Returns the total number of pages allocated on disk
     */
    public long getTotalDiskPages() {
        return DBComponent.DISK_MANAGER().getNumPages();
    }

    /**
     * Pins a page to the buffer
     * @param pageId id of the page to pin
     */
    public void pin(PageId pageId) {
        BufferFrame found = findFrameByPageId(pageId);
        if(found != null) {
            found.pin();
        }
    }

    /**
     * Unpins a page from the buffer.
     * @param pageId id of the page to unpin
     * @param isDirty whether the page was modified
     */
    public void unpin(PageId pageId, boolean isDirty) {
        BufferFrame found = findFrameByPageId(pageId);
        if(found != null) {
            if(isDirty)
                found.setDirty(true);
            found.unpin();
        }
    }

    /**
     * Finds the frame containing the pageId in the buffer
     * @param pageId the page id to find in the buffer
     * @return the found BufferFrame, or null
     */
    private BufferFrame findFrameByPageId(PageId pageId) {
        for(BufferFrame frame : mBufferFrames) {
            if(frame.contains(pageId)) {
                return frame;
            }
        }
        return null;
    }

    /**
     * Gets the next buffer frame to be replaced
     * If we haven't allocated all frames yet, we return a new frame
     * If all frames have been allocated, we choose one according to the replacement policy
     */
    private BufferFrame getBufferFrame() {
        if(mBufferFrames.size() < MAX_BUFFER_FRAMES) {
            BufferFrame frame = new BufferFrame();
            mBufferFrames.add(frame);
            return frame;
        }
        return mReplacementPolicy.choose(mBufferFrames);
    }

    /**
     * Returns the number of cache hits (for statistics)
     */
    public int getCacheHits() {
        return cacheHits;
    }

    /**
     * Returns the number of pinned pages
     */
    public int getNumberOfPinnedPages() {
        int i = 0;
        for(BufferFrame frame : mBufferFrames) {
            if(frame.isPinned()) {
                i++;
            }
        }
        return i;
    }

    /**
     * Returns a Queue of page accesses
     */
    public Queue<Integer> getPageAccesses() {
        return pageAccesses;
    }
}

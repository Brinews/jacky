package buffer;

import disk.Page;
import disk.PageId;

/**
 * Describes the contents of a buffer
 * Contains a Page and a PageId
 */
public class BufferFrame {
    private Page content;
    private PageId pageId;
    private boolean isDirty;
    private int pinned;
    private int clockCount;

    public BufferFrame() {
        content = new Page();
        pageId = new PageId();
        isDirty = false;
        pinned = 0;
        clockCount = 0;
    }

    // checks whether current frame is valid or empty
    public boolean isEmpty() {
        return (this.pageId == null);
    }

    // checks whether the current frame is dirty (content changed) or not
    public boolean isDirty() {
        return this.isDirty;
    }

    // marks the current frame as dirty
    public void setDirty(boolean isDirty) {
        this.isDirty = isDirty;
    }

    // checks whether the current frame is pinned or not
    public boolean isPinned() {
        return (this.pinned > 0);
    }

    // marks the current frame as pinned (automatically done as part of getPage)
    public void pin() {
        this.pinned++;
    }

    // reduces the current frame's pin count (but not if it is already 0)
    public void unpin() {
        if (this.isPinned())
            this.pinned--;
    }

    // checks whether this frame contains a certain page
    public boolean contains(PageId pid) {
        return pageId.equals(pid);
    }

    public PageId getPageId() {
        return new PageId(pageId.get());
    }

    // gets the Page content of current frame
    // Important: this pins the page at the same time!
    // one needs to call unpin after usage
    public Page getPage() {
        this.pin();
        return this.content;
    }

    // stores a given Page in this frame
    public void setPage(PageId pid, Page page) {
        if (this.isDirty())
            throw new BufferFrameDirtyException();
        this.content.copy(page);
        this.pageId.set(pid.get());
        this.pinned = 0;
        this.isDirty = false;
        this.clockCount = 0;
    }

    public int getClockCount() {
        return clockCount;
    }

    public void setClockCount(int clockCount) {
        this.clockCount = clockCount;
    }

    public static class BufferFrameDirtyException extends RuntimeException {}
    public static class AllBufferFramesPinnedException extends RuntimeException {}
}

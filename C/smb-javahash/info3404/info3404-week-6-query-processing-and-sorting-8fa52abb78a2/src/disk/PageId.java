package disk;

import global.DatabaseConstants;

/**
 * Page ID
 * - Identifies a page on disk
 */
public class PageId {

    private int pageId;

    public PageId() {
        this(DatabaseConstants.INVALID_PAGE_ID);
    }

    public PageId(int pageId) {
        this.pageId = pageId;
    }

    /**
     * Checks the validity of the Page ID
     * @return true if the page is valid, false if it isn't
     */
    public boolean isValid() {
        return pageId != DatabaseConstants.INVALID_PAGE_ID;
    }

    /**
     * Gets the integer associated with the Page ID
     */
    public int get() {
        return pageId;
    }

    /**
     * Sets the integer associated with the Page ID
     */
    public void set(int value) {
        pageId = value;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof PageId)) return false;

        PageId pageId = (PageId) o;

        return (this.pageId == pageId.pageId);
    }

    /**
     * Allows PageId instances to be used in HashMaps properly
     */
    @Override
    public int hashCode() {
        return pageId;
    }

    @Override
    public String toString() {
        return "PageId{" +
                "pageId=" + pageId +
                '}';
    }
}

package global;

import buffer.BufferManager;
import buffer.policy.*;
import disk.DiskManager;

import java.io.IOException;

/**
 * Database Component Singletons are initialised and stored in this class.
 */
public class DBComponent {

    private static boolean mInitialised;
    private static DiskManager mDiskManager;
    private static BufferManager mBufferManager;
    private static Catalog mCatalog;

    public static void initialiseComponents() {
        if(!mInitialised) {
            mBufferManager = new BufferManager(new GeneralisedClockReplacementPolicy(1));
            try {
                mDiskManager = new DiskManager(DatabaseConstants.DEFAULT_DB_NAME);
            } catch (IOException e) {}
            mCatalog = new Catalog();
            mInitialised = true;
        }
        // Do nothing
    }

    public static void resetComponents() {
        mInitialised = false;
        initialiseComponents();
    }

    public static Catalog CATALOG() {
        if(mCatalog == null) {
            throw new ComponentsNotInitialisedError();
        }
        return mCatalog;
    }

    public static DiskManager DISK_MANAGER() {
        if(mDiskManager == null) {
            throw new ComponentsNotInitialisedError();
        }
        return mDiskManager;
    }

    public static BufferManager BUFFER_MANAGER() {
        if(mBufferManager == null) {
            throw new ComponentsNotInitialisedError();
        }
        return mBufferManager;
    }

    public static class ComponentsNotInitialisedError extends Error {}
}

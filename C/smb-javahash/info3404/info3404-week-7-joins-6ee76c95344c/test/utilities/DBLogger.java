package utilities;

import global.DBComponent;

/**
 * Helper class to simplify accessing statistics from the database
 */
public class DBLogger {

    private int startingPageAccesses;
    private int finishingPageAccesses;

    public DBLogger() {
        startingPageAccesses = 0;
        finishingPageAccesses = 0;
    }

    public void start() {
        startingPageAccesses = DBComponent.BUFFER_MANAGER().getPageAccesses().size();
    }

    public void stop() {
        finishingPageAccesses = DBComponent.BUFFER_MANAGER().getPageAccesses().size();
    }

    public int getPageAccesses() {
        return finishingPageAccesses - startingPageAccesses;
    }
}

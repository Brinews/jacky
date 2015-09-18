package utilities;

import access.read.AccessIterator;
import access.write.AccessInserter;
import heap.HeapFile;
import heap.HeapPage;

/**
 * Helper utilities for an AccessIterator and HeapFile to assist with testing
 */
public class TestingUtils {

    public static int count(AccessIterator rows) {
        int i = 0;
        while(rows.hasNext()) {
            rows.next();
            i++;
        }
        rows.close();
        return i;
    }

    public static int count(HeapFile table) {
        try(AccessIterator iterator = table.iterator()) {
            return count(iterator);
        }
    }

    public static int countPages(HeapFile table) {
        try(AccessIterator iterator = table.iterator()) {
            int spaceOnPage = HeapPage.PAGE_SIZE- HeapPage.RECORD_START_INDEX;
            int tuplesOnPage = (int)(spaceOnPage / (double) iterator.getSchema().getTupleLength());
            int i = 0;
            while(iterator.hasNext()) {
                if(iterator.next() != null) {
                    i += 1;
                };
            }
            return (int)Math.ceil(i / (double)tuplesOnPage);
        }
    }

    public static void insertRows(HeapFile table, Object[][] rows) {
        try(AccessInserter inserter = table.inserter()) {
            for(Object[] row : rows) {
                inserter.insert(row);
            }
        }
    }


}

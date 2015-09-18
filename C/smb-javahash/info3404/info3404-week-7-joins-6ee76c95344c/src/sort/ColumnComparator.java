package sort;

import heap.Tuple;

import java.util.Comparator;

/**
 * Compares two tuples, based on the columns that are given in the constructor
 * When used in conjunction with a sort method, the tuples will end up in sorted ascending order
 */
public class ColumnComparator implements Comparator<Tuple> {

        private final String[] columns;

        public ColumnComparator(String[] columns) {
            this.columns = columns;
        }

        @Override
        public int compare(Tuple o1, Tuple o2) {
            for (String column : columns) {
                Object leftValue = o1.getColumn(column);
                Object rightValue = o2.getColumn(column);
                int compare = 0;
                switch (o1.getSchema().getFieldType(column)) {
                    case INTEGER:
                        compare = Integer.compare((Integer) leftValue, (Integer) rightValue);
                        break;
                    case DOUBLE:
                        compare = Double.compare((Double) leftValue, (Double) rightValue);
                        break;
                    case STRING:
                        compare = ((String) leftValue).compareTo((String) rightValue);
                        break;
                    case BOOLEAN:
                        compare = Boolean.compare((Boolean) leftValue, (Boolean) rightValue);
                        break;
                }
                if (compare != 0)
                    return compare;
            }
            return 0;
        }
    }
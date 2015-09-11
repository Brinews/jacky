package filter;

import heap.Tuple;

/**
 * Allows you to inverse other predicates
 */
public class NotModifier implements Filter.Predicate {

    private Filter.Predicate predicate;

    public NotModifier(Filter.Predicate originalPredicate) {
        this.predicate = originalPredicate;
    }

    @Override
    public boolean matchesCondition(Tuple row, String columnName) {
        return ! (predicate.matchesCondition(row, columnName));
    }

    @Override
    public boolean shouldStopSearching(Tuple row, String columnName) {
        return false;
    }
}

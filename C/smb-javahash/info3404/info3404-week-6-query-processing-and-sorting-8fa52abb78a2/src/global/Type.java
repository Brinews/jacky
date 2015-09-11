package global;

/**
 * Type Enum Class
 * Lists the supported types by the Database
 */
public enum Type {
    STRING(String.class) {
        @Override
        public int getLen() {
            return 2 + DatabaseConstants.MAX_STRING_LENGTH;
        }
    },
    DOUBLE(Double.class) {
        @Override
        public int getLen() {
            return 8;
        }
    },
    INTEGER(Integer.class) {
        @Override
        public int getLen() {
            return 4;
        }
    },
    BOOLEAN(Boolean.class) {
        @Override
        public int getLen() {
            return 1;
        }
    };

    private Class<?> typeClass;

    Type(Class<?> typeClass) {
        this.typeClass = typeClass;
    }

    public abstract int getLen();

    public Class<?> getTypeClass() {
        return typeClass;
    }

    // Converts a String to the appropriate Object type
    public Object parseType(String value) {
        switch(this) {
            case STRING:
                return value;
            case DOUBLE:
                try { return Double.parseDouble(value); } catch(Exception e) {return false; }
            case INTEGER:
                try { return Integer.parseInt(value); } catch(Exception e) {return false; }
            case BOOLEAN:
                return (value.equals("true") || value.equals("false") ? value.equals("true") : null);
            default:
                return null;
        }
    }
}

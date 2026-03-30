package client.protocol;

public class Args {

    // assume char is used to represent a raw byte
    public enum Type{
        STRING,
        INT32,
        INT64,
        FLOAT,
        CHAR
    }
    
    public Type type;
    public Object value;
    
    public Args(Type type, Object value) {
        this.type = type;
        this.value = value;
    }
    
    public static Args ofString(String value) {
        return new Args(Type.STRING, value);
    }
    public static Args ofInt32(int value) {
        return new Args(Type.INT32, value);
    }
    public static Args ofInt64(long value) {
        return new Args(Type.INT64, value);
    }
    public static Args ofFloat(float value) {
        return new Args(Type.FLOAT, value);
    }
    public static Args ofChar(char value) {
        return new Args(Type.CHAR, value);
    }
    
}


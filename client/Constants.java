package client;

public final class Constants {
    // Argument Constant
    public static final String DEFAULT_HOST = "127.0.0.1";
    public static final int DEFAULT_PORT = 8080;
    public static final double DEFAULT_FAILURE_RATE = 0.0;
    public static final int DEFAULT_TIMEOUT = 1000;
    public static final int DEFAULT_NO_TIMEOUT = 0;
    public static final int DEFAULT_MAX_TIMEOUT = 0;
    public static final int DEFAULT_MAX_RETRY = 3;

    //connection
    public static final int ACK = 1;
    public static final String ACK_CHAR = "1";
    public static final int NAK = 0;
    public static final String NAK_CHAR = "0";
    public static final int RESPONSE_TYPE_SIZE = 1;
    public static final String INVALID_RESPONSE = "server problem";
    public static final String TIMEOUT_MSG = "timeout resending..(%d/%d)%n";
    public static final String MAX_TIMEOUT_MSG = "TLE bro";

    //semantic
    public static final int NO_SEM_INVO = 0;
    public static final int AT_LEAST_ONE_SEM_INVO = 1;
    public static final int AT_MOST_ONE_SEM_INVO = 2;

    //type
    public static final int INT_SIZE = 4;
    public static final int FLOAT_SIZE = 4;
    public static final int LONG_SIZE = 8;
    public static final int CHAR_SIZE = 1;
    public static final int BUFFER_SIZE = 2048;

    //service 
    public static final int SERVICE_OPEN_ACCOUNT = 1;
    public static final int SERVICE_CLOSE_ACCOUNT = 2;
    public static final int SERVICE_DEPOSIT_WITHDRAW_MONEY = 3;
    public static final int SERVICE_MONITOR_UPDATE = 4;
    public static final int SERVICE_VIEW_ACCOUNT = 5;
    public static final int SERVICE_TRANSFER_MONEY = 6;
    public static final int SERVICE_CHANGE_PASSWORD = 7;
    public static final int SERVICE_EXIT = 8;
    public static final int SERVICE_MONITOR_LIVE = 9;
    public static final int SERVICE_MONITOR_END = 10;

    //currency
    public static final char SGD = 0;
    public static final char USD = 1;
    public static final char EUR = 2;
    

    private Constants() {}
}
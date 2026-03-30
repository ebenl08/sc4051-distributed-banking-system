package client.protocol;



public class Request {
    public final int requestId;
    public final int serviceId;
    public final Args[] payload;

    public Request(int requestId, int serviceId, Args[] payload) {
        this.requestId = requestId;
        this.serviceId = serviceId;
        this.payload = payload;
    }
    
}
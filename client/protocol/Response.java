package client.protocol;



public class Response {
    public final int responseId;
    public final int statusCode;
    public final Args[] payload;

    public Response(int responseId, int statusCode, Args[] payload) {
        this.responseId = responseId;
        this.statusCode = statusCode;
        this.payload = payload;
    }

    public boolean isSuccess() {
        return this.statusCode == StatusCode.SUCCESS;
    }
    public boolean isFailure() {
        return !isSuccess();
    }
    
}

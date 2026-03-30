package client.net;

import client.Constants;

import java.io.IOException;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.TimeoutException;

import client.protocol.Args;
import client.protocol.Request;
import client.protocol.Response;
import client.protocol.ServiceSchema;
import client.protocol.ServiceSchemaFactory;
import client.protocol.StatusCode;


public class UDPClient {
    private int semanticInvoc;
    private int requestIdCounter;
    private int timeoutMs;
    private int maxRetry;
    private boolean debug;

    private final Map<Integer, Boolean> requestIdHistory;
    private final Socket socket;
    
    private ServiceSchema serviceSchema;

    public interface Callback {
        void onCallback(Request callbackRequest);
    }
    
    public UDPClient() throws SocketException, UnknownHostException {
        this.requestIdCounter = 0;
        this.requestIdHistory = new HashMap<>();
        
        this.semanticInvoc = Constants.NO_SEM_INVO;
        this.timeoutMs = Constants.DEFAULT_TIMEOUT;
        this.maxRetry = Constants.DEFAULT_MAX_RETRY;
        this.debug = false;
        this.socket = new Socket();
        this.socket.setTimeoutMs(this.timeoutMs);
    }

    private UDPClient(
        int semanticInvoc, 
        int timeoutMs, 
        int maxRetry,
        boolean debug,
        Socket socket
    ) throws SocketException, UnknownHostException {
        this.socket = socket;
        this.semanticInvoc = semanticInvoc;
        this.timeoutMs = timeoutMs;
        this.maxRetry = maxRetry;
        this.socket.setTimeoutMs(timeoutMs);
        this.debug = debug;
        this.requestIdCounter = 0;
        this.requestIdHistory = new HashMap<>();
    }

    // Getters
    public int getSemanticInvoc() {
        return this.semanticInvoc;
    }
    public int getTimeoutMs() {
        return this.timeoutMs;
    }
    public int getMaxRetry() {
        return this.maxRetry;
    }
    public int getRequestId() {
        return this.requestIdCounter;
    }
    public boolean isDebug() {
        return this.debug;
    }
    public boolean isSemanticInvocAtMostOne() {
        return this.semanticInvoc == Constants.AT_MOST_ONE_SEM_INVO;
    }
    public boolean isSemanticInvocAtLeastOne() {
        return this.semanticInvoc == Constants.AT_LEAST_ONE_SEM_INVO;
    }
    public boolean isSemanticInvocNone() {
        return this.semanticInvoc == Constants.NO_SEM_INVO;
    }
    
    // Setters
    public void setSemanticInvoc(int semanticInvoc) {
        this.semanticInvoc = semanticInvoc;
    }
    public void setTimeoutMs(int timeoutMs) {
        this.timeoutMs = timeoutMs;
        try {
            this.socket.setTimeoutMs(timeoutMs);
        } catch (SocketException e) {
            if(debug) {System.err.println("Failed to set socket timeout: " + e.getMessage());}
        }
    }
    public void setMaxRetry(int maxRetry) {
        this.maxRetry = maxRetry;
    }
    public void setDebug(boolean debug) {
        this.debug = debug;
    }

    public boolean isDuplicateRequest(int requestId) {
        return this.requestIdHistory.getOrDefault(requestId, false);
    }
    public boolean isRequestOutdated(int requestId) {
        return requestId < this.requestIdCounter;
    }


    public void validateRequestSchema(Args[] args, Args.Type[] expectedTypes) {
        if (args.length != expectedTypes.length) {
            throw new IllegalArgumentException("Argument count mismatch. Expected " + expectedTypes.length + " but got " + args.length);
        }
        for (int i = 0; i < args.length; i++) {
            if (args[i].type != expectedTypes[i]) {
                throw new IllegalArgumentException("Argument type mismatch at index " + i + ". Expected " + expectedTypes[i] + " but got " + args[i].type);
            }
        }
    }

    public static String bytesToHex(byte[] bytes) {
        StringBuilder sb = new StringBuilder();
        for (byte b : bytes) {
            sb.append(String.format("%02X", b));
        }
        return sb.toString();
    }   
    
    public Response sendRequest(int serviceId, Args[] args) throws IOException, TimeoutException {
        
        this.serviceSchema = ServiceSchemaFactory.getServiceSchema(serviceId);
        validateRequestSchema(args, this.serviceSchema.inputSchema);

        int requestId = getRequestId();
        Request request = new Request(requestId, serviceId, args);
        byte[] requestBytes = PacketUtils.buildRequestMessage(request);
        System.out.println(bytesToHex(requestBytes));
        
        int retryCount = 0;
        byte[] response = null;
        
        // This sends the request and waits for a response
        while (retryCount <= this.maxRetry) {
            try {
                this.socket.send(requestBytes);
                response = this.socket.receive();
                break;
            } catch (SocketTimeoutException e) {
                retryCount++;
            } 
        }
        if(retryCount > this.maxRetry){
            throw new TimeoutException(String.format("Request ID %d failed after %d retries", requestId, this.maxRetry));
        }
        return PacketUtils.parseResponseMessage(response, this.serviceSchema);
    }

    public void checkRequest(Response response) {
        int requestId = response.responseId;
        if (isDuplicateRequest(requestId)) {
            if(debug) {System.out.println("Received duplicate request with ID: " + requestId);}
            return;
        }
        this.requestIdHistory.put(requestId, true);
    }

    public void monitorServer(Callback callback) throws IOException, InterruptedException {

        while (true) {
            try {
                byte[] callbackMessage = this.socket.receive();
                Request callbackRequest = PacketUtils.parseCallbackMessage(callbackMessage);

                int callbackId = callbackRequest.requestId;
                if( isSemanticInvocAtMostOne()
                        && (isDuplicateRequest(callbackId) 
                        || isRequestOutdated(callbackId))){
                    if(debug){System.out.println("Received duplicate callback request with ID: " + callbackRequest.requestId);}
                    continue;
                }else {
                    this.requestIdHistory.put(callbackRequest.requestId, true);
                    requestIdHistory.put(callbackRequest.requestId, true);   
                    callback.onCallback(callbackRequest);
                }

                Response callbackResponse = new Response(
                    callbackRequest.requestId,
                    StatusCode.SUCCESS,
                    new Args[0] // For simplicity, we assume callback response has no payload
                );
                byte[] callbackResponseBytes = PacketUtils.buildResponseMessage(callbackResponse);
                this.socket.send(callbackResponseBytes);


                byte[] responseBytes = PacketUtils.buildResponseMessage(callbackResponse);
                this.socket.send(responseBytes);
            } catch (SocketTimeoutException e) {
                continue; // Continue waiting for callbacks
            }
        }
    }


    public static class Builder {
        private int semanticInvoc = Constants.NO_SEM_INVO;
        private int timeoutMs = Constants.DEFAULT_TIMEOUT;
        private int maxRetry = Constants.DEFAULT_MAX_RETRY;
        private boolean debug = false;
        private Socket socket;

        public Builder setSemanticInvoc(int semanticInvoc) {
            this.semanticInvoc = semanticInvoc;
            return this;
        }

        public Builder setTimeoutMs(int timeoutMs) {
            this.timeoutMs = timeoutMs;
            return this;
        }

        public Builder setMaxRetry(int maxRetry) {
            this.maxRetry = maxRetry;
            return this;
        }

        public Builder setDebug(boolean debug) {
            this.debug = debug;
            return this;
        }

        public Builder setSocket(Socket socket) {
            this.socket = socket;
            return this;
        }
        public UDPClient build() throws SocketException, UnknownHostException {
            if (this.socket == null) {
                this.socket = new Socket();
            }
            return new UDPClient(semanticInvoc, timeoutMs, maxRetry, debug, socket);
        }
    }
    

}

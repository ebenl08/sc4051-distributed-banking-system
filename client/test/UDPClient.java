package test;

import java.io.IOException;
import java.net.*;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.TimeoutException;

public class UDPClient{

  //constants
  public static class C{
    public static final String SERVER_IP = "localhost";
    public static final int SERVER_PORT = 8080;

    public static final int BUFFER_SIZE = 2048;
    public static final int INT_SIZE = 4;
    public static final int LONG_SIZE = 8;
    public static final int FLOAT_SIZE = 4;

    public static final int SEMANTIC_NONE = 0;
    public static final int SEMANTIC_AT_LEAST_ONCE = 1;
    public static final int SEMANTIC_AT_MOST_ONCE = 2;

    public static final int DEFAULT_TIMEOUT_MS = 1000;
    public static final int DEFAULT_MAX_RETRY = 3;
    public static final String ACK = "1";
  }

  private final DatagramSocket socket;
  private final InetAddress servaddr;
  private int semanticInvoc;
  private int requestIdCounter;
  private int timeoutMs;
  private int maxRetry;

  private final Map<Integer, Boolean> handledResponseIds;

  private boolean debug;

  public UDPClient() throws SocketException, UnknownHostException{
    this.socket = new DatagramSocket();
    this.servaddr = InetAddress.getByName(C.SERVER_IP);
    this.semanticInvoc = C.SEMANTIC_NONE;
    this.requestIdCounter = 0;
    this.timeoutMs = C.DEFAULT_TIMEOUT_MS;
    this.maxRetry = C.DEFAULT_MAX_RETRY;
    this.handledResponseIds = new HashMap<>();
    this.debug = false;
    this.socket.setSoTimeout(this.timeoutMs);
  }

  public void setDebug(boolean debug){
    this.debug = debug;
  }

  public void setSemanticInvoc(int semanticInvoc){
    this.semanticInvoc = semanticInvoc;
  }

  public void setTimeoutMs(int timeoutMs) throws SocketException{
    this.timeoutMs = timeoutMs;
    this.socket.setSoTimeout(timeoutMs);
  }

  public void setMaxRetry(int maxRetry){
    this.maxRetry = maxRetry;
  }

  public int getNextRequestId(){
    this.requestIdCounter += 1;
    return this.requestIdCounter;
  }

  private void sendAck(int responseId) throws IOException {
    // [CHANGED] ACK now uses raw UDP payload (no framing header).
    byte[] ackPayload = utils.buildAckPayload(responseId);
    send(ackPayload);
  }

  private byte[] receiveResponse(boolean monitorMode) throws IOException {
    // [NEW] Semantic-aware response handling with duplicate protection.
    while(true){
      byte[] payload = receive();
      utils.RequestHeader header = utils.parseRequestHeader(payload);
      int responseId = header.requestId;
      int bodyOffset = C.INT_SIZE * 3;

      if(this.debug) System.out.println("[DEBUG] received response id=" + responseId);

      if(this.semanticInvoc >= C.SEMANTIC_AT_MOST_ONCE && this.handledResponseIds.containsKey(responseId)){
        if(this.debug) System.out.println("Duplicate response, ACK resent");
        sendAck(responseId);
        continue;
      }

      if(this.semanticInvoc >= C.SEMANTIC_AT_MOST_ONCE || (monitorMode && this.semanticInvoc >= C.SEMANTIC_AT_LEAST_ONCE)){
        sendAck(responseId);
      }

      this.handledResponseIds.put(responseId, true);
      return java.util.Arrays.copyOfRange(payload, bodyOffset, payload.length);
    }
  }

  public byte[] sendRequest(byte[] requestPayload, boolean monitorMode) throws IOException, TimeoutException {
    int timeoutCount = 0;

    while(true){
      try{
        send(requestPayload);
        return receiveResponse(monitorMode);
      } catch (SocketTimeoutException e){
        timeoutCount += 1;
        System.out.println("Timeout. Retrying request (" + timeoutCount + "/" + this.maxRetry + ")");

        if(this.maxRetry > 0 && timeoutCount >= this.maxRetry){
          throw new TimeoutException("Max retry reached");
        }

        if(this.semanticInvoc < C.SEMANTIC_AT_LEAST_ONCE){
          throw e;
        }
      }
    }
  }

  public byte[] sendRequestByService(int serviceId, int statusCode, boolean monitorMode, Object... fields)
      throws IOException, TimeoutException {
    int requestId = getNextRequestId();
    byte[] requestPayload = utils.buildRequest(requestId, serviceId, statusCode, fields);
    return sendRequest(requestPayload, monitorMode);
  }

  public List<utils.Argument> sendRequestByServiceAndDecode(
      int serviceId, int statusCode, boolean monitorMode, Object... fields)
      throws IOException, TimeoutException {
    byte[] responseBody = sendRequestByService(serviceId, statusCode, monitorMode, fields);
    int[] counterRef = new int[] {0};
    return utils.decodeByService(responseBody, counterRef, serviceId);
  }

 

  public void send(byte[] message) throws IOException, SocketTimeoutException {
    byte[] buffer = message;
    DatagramPacket packet = new DatagramPacket(buffer, buffer.length, this.servaddr, C.SERVER_PORT);
    this.socket.send(packet);
  }
  
  public byte[] receive() throws IOException, SocketTimeoutException{
    byte[] buffer = new byte[C.BUFFER_SIZE];
    DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
    this.socket.receive(packet);
    return java.util.Arrays.copyOf(packet.getData(), packet.getLength());
  }
  
}

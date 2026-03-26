package client;

import java.io.IOException;
import java.net.*;
import java.util.concurrent.TimeoutException;

public class socket{

  private final DatagramSocket socket;
  private final InetAddress servaddr;
  private final UDPClient communication;

  public socket(UDPClient communication) throws SocketException, UnknownHostException{
    this.communication = communication;
    this.socket = new DatagramSocket();
    this.servaddr = InetAddress.getByName(Constants.DEFAULT_HOST);
    this.socket.setSoTimeout(this.communication.getTimeoutMs());
  }

  public socket() throws SocketException, UnknownHostException {
    this(new UDPClient());
  }

  public void setTimeoutMs(int timeoutMs) throws SocketException{
    this.socket.setSoTimeout(timeoutMs);
  }



  public byte[] receiveResponse(boolean monitorMode) throws IOException {
    while (true) {
      byte[] buffer = new byte[Constants.BUFFER_SIZE];
      DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
      this.socket.receive(packet);
      byte[] payload = java.util.Arrays.copyOf(packet.getData(), packet.getLength());
      utils.RequestHeader header = utils.parseRequestHeader(payload);
      int responseId = header.requestId;
      int bodyOffset = Constants.INT_SIZE * 3;

      if (this.communication.isDebug()) {
        System.out.println("[DEBUG] received response id=" + responseId);
      }

      UDPClient.ResponseDecision decision = this.communication.evaluateResponse(responseId, monitorMode);

      if (decision.shouldAck) {
        byte[] ackPayload = utils.buildAckPayload(responseId);
        DatagramPacket ackPacket = new DatagramPacket(
            ackPayload,
            ackPayload.length,
            this.servaddr,
            Constants.DEFAULT_PORT);
        this.socket.send(ackPacket);
      }

      if (decision.duplicate) {
        if (this.communication.isDebug()) {
          System.out.println("Duplicate response, ACK resent");
        }
        continue;
      }

      return java.util.Arrays.copyOfRange(payload, bodyOffset, payload.length);
    }
  }

  public byte[] sendRequest(byte[] requestPayload, boolean monitorMode) throws IOException, TimeoutException {
    int timeoutCount = 0;

    while (true) {
      try {
        setTimeoutMs(this.communication.getTimeoutMs());
        DatagramPacket requestPacket = new DatagramPacket(
            requestPayload,
            requestPayload.length,
            this.servaddr,
            Constants.DEFAULT_PORT);
        this.socket.send(requestPacket);
        return receiveResponse(monitorMode);
      } catch (SocketTimeoutException e) {
        timeoutCount += 1;
        try {
          this.communication.shouldRetryAfterTimeout(timeoutCount);
        } catch (SocketTimeoutException semanticTimeout) {
          throw e;
        }
      }
    }
  }
  
}

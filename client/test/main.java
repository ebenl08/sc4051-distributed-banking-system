package test;

import java.net.SocketException;
import java.net.UnknownHostException;

public class main {
  public static void main(String[] args) {
    try {
      communication client = new communication();


      client.setDebug(true);
      client.setSemanticInvoc(communication.C.SEMANTIC_AT_LEAST_ONCE);
      client.setTimeoutMs(1000);
      client.setMaxRetry(5);

      System.out.println("Communication layer entrypoint is ready.");
      System.out.println("Client initialized with server " + UDPClient.C.SERVER_IP + ":" + UDPClient.C.SERVER_PORT);
    } catch (SocketException e) {
      System.out.println("Socket error: " + e.getMessage());
    } catch (UnknownHostException e) {
      System.out.println("Host error: " + e.getMessage());
    }
  }
}

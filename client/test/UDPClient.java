package test;

import java.io.IOException;
import java.net.*;

public class UDPClient{

  //constants
  public static class C{
    public static final String SERVER_IP = "localhost";
    public static final int SERVER_PORT = 8080;

    public static final int BUFFER_SIZE = 2048;
    public static final int INT_SIZE = 4;
    public static final int LONG_SIZE = 8;
    public static final int FLOAT_SIZE = 4;

    public static final int DEFAULT_TIMEOUT_MS = 1000;
    public static final String ACK = "1";
  }

  private final DatagramSocket socket;
  private final InetAddress servaddr;

  public UDPClient() throws SocketException, UnknownHostException{
    this.socket = new DatagramSocket();
    this.servaddr = InetAddress.getByName(C.SERVER_IP);
    this.socket.setSoTimeout(C.DEFAULT_TIMEOUT_MS);
  }

  public void setTimeoutMs(int timeoutMs) throws SocketException{
    this.socket.setSoTimeout(timeoutMs);
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

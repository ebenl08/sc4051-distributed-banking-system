package client;

import java.io.IOException;
import java.net.*;

public class socket{

  private final DatagramSocket socket;
  private final InetAddress servaddr;

  public socket() throws SocketException, UnknownHostException{
    this.socket = new DatagramSocket();
    this.servaddr = InetAddress.getByName(Constants.DEFAULT_HOST);
    this.socket.setSoTimeout(Constants.DEFAULT_TIMEOUT);
  }

  public void setTimeoutMs(int timeoutMs) throws SocketException{
    this.socket.setSoTimeout(timeoutMs);
  }

  public void send(byte[] payload) throws IOException {
    DatagramPacket requestPacket = new DatagramPacket(
        payload,
        payload.length,
        this.servaddr,
        Constants.DEFAULT_PORT);
    this.socket.send(requestPacket);
  }

  public byte[] receive() throws IOException {
    byte[] buffer = new byte[Constants.BUFFER_SIZE];
    DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
    this.socket.receive(packet);
    return java.util.Arrays.copyOf(packet.getData(), packet.getLength());
  }
  
}

package client;

import java.io.IOException;
import java.net.*;

public class UDPClient{

  public class C{
    private static final int AT_MOST_ONCE = 0;
    private static final int AT_LEAST_ONCE = 1;
    private static final int BUFFER_SIZE = 1500;
    private static final int MAX_TIMEOUT = 5;
  }

  private final DatagramSocket socket;
  private final InetAddress servaddr;
  private final int port;
  // private int semanticInvoc; // will be mainly used for monitor update
  // private int msgIdCounter;
  // private int maxTimeout;

  private boolean debug;

  public UDPClient(String ip, int port) throws SocketException, UnknownHostException{
    this.socket = new DatagramSocket();
    this.servaddr = InetAddress.getByName(ip);
    this.port = port;

    // this.setTimeout(C.MAX_TIMEOUT);
    // this.msgIdCounter = 0;
  }

  // private void setTimeout(int maxTimeout) throws SocketException{
  //   this.socket.setSoTimeout(maxTimeout);
  //   this.maxTimeout = maxTimeout;
  // }

  public void send(byte[] message) throws IOException, SocketTimeoutException {
    byte[] buffer = message;
    DatagramPacket packet = new DatagramPacket(buffer, buffer.length, this.servaddr, this.port);
    this.socket.send(packet);
  }
  
  public byte[] receive() throws IOException, SocketTimeoutException{
    byte[] buffer = new byte[C.BUFFER_SIZE];
    DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
    this.socket.receive(packet);
    return packet.getData();
  }
  
  public static void main(String[] args) {
    try{
      UDPClient client = new UDPClient("localhost", 8080);
      //String message = "Hello from Client";
      //client.send(message.getBytes());
      
      byte[] message = new byte[]{
        1, 0, 0, 0,   // reqID = 1
        0, 0, 0, 0,   // serviceID = 0
        0, 0, 0, 0,   // statusCode = 0

        5, 0, 0, 0,   'A','l','i','c','e',

        11, 0, 0, 0,  'p','a','s','s','w','o','r','d','1','2','3',

        'M',

        -24, 3, 0, 0, 0, 0, 0, 0   // INT64 1000
      };
      client.send(message);

      byte[] response = client.receive();
      System.out.println(new String(response)); 
      
    } catch (SocketException e) {
      
    } catch (IOException e){
      
    }
  }

}
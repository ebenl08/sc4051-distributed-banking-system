package client.net.archive;

import client.Constants;

import java.io.IOException;
import java.net.*;

public class Socket{

  private final DatagramSocket socket;
  private final InetAddress servaddr;
  private final UDPClient communication;
  private int port;

  public Socket(UDPClient communication) throws SocketException, UnknownHostException{
    this.communication = communication;
    this.socket = new DatagramSocket();
    this.servaddr = InetAddress.getByName(Constants.DEFAULT_HOST);
    this.socket.setSoTimeout(this.communication.getTimeoutMs());
  }

  public Socket() throws SocketException, UnknownHostException {
    this(new UDPClient());
  }

  public void setTimeoutMs(int timeoutMs) throws SocketException{
    this.socket.setSoTimeout(timeoutMs);
  }
  
  public void send(byte[] message) throws IOException, SocketTimeoutException {
     byte[] buffer = message;
     DatagramPacket packet = new DatagramPacket(buffer, buffer.length, this.servaddr, this.port);
     this.socket.send(packet);
   }
   
  public byte[] receive() throws IOException {
    byte[] buffer = new byte[Constants.BUFFER_SIZE];
    DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
    this.socket.receive(packet);
    return java.util.Arrays.copyOf(packet.getData(), packet.getLength());
  }
  
}




// package client.net;

// import java.io.IOException;
// import java.net.*;

// public class Socket{

//   public class C{
//     private static final int AT_MOST_ONCE = 0;
//     private static final int AT_LEAST_ONCE = 1;
//     private static final int BUFFER_SIZE = 1500;
//     private static final int MAX_TIMEOUT = 5;
//   }

//   private final DatagramSocket socket;
//   private final InetAddress servaddr;
//   private final int port;
//   // private int semanticInvoc; // will be mainly used for monitor update
//   // private int msgIdCounter;
//   // private int maxTimeout;

//   private boolean debug;

//   public Socket(String ip, int port) throws SocketException, UnknownHostException{
//     this.socket = new DatagramSocket();
//     this.servaddr = InetAddress.getByName(ip);
//     this.port = port;

//     // this.setTimeout(C.MAX_TIMEOUT);
//     // this.msgIdCounter = 0;
//   }

//   // private void setTimeout(int maxTimeout) throws SocketException{
//   //   this.socket.setSoTimeout(maxTimeout);
//   //   this.maxTimeout = maxTimeout;
//   // }

//   public void send(byte[] message) throws IOException, SocketTimeoutException {
//     byte[] buffer = message;
//     DatagramPacket packet = new DatagramPacket(buffer, buffer.length, this.servaddr, this.port);
//     this.socket.send(packet);
//   }
  
//   public byte[] receive() throws IOException, SocketTimeoutException{
//     byte[] buffer = new byte[C.BUFFER_SIZE];
//     DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
//     this.socket.receive(packet);
//     return packet.getData();
//   }
  
//   public static void main(String[] args) {
//     try{
//       Socket client = new Socket("localhost", 8080);
//       String message = "Hello from Client";
//       client.send(message.getBytes());
      
//       byte[] response = client.receive();
//       System.out.println(new String(response)); 
      
//     } catch (SocketException e) {
      
//     } catch (IOException e){
      
//     }
//   }

// }
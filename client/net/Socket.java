package client.net;

import java.net.*;

import client.Constants;
import java.io.IOException;
import java.util.Arrays;


public class Socket {
    private final DatagramSocket socket;
    private final InetAddress servaddr;
    private final int servport;

    public Socket(String ip, int port) throws SocketException, UnknownHostException{
        this.socket = new DatagramSocket();
        this.servaddr = InetAddress.getByName(ip);
        this.servport = port;
        this.setTimeoutMs(Constants.DEFAULT_TIMEOUT);
    }

    public Socket() throws SocketException, UnknownHostException {
        this(Constants.DEFAULT_HOST, Constants.DEFAULT_PORT);
    }

    public void setTimeoutMs(int timeoutMs) throws SocketException{
        this.socket.setSoTimeout(timeoutMs);
    }
    
    public void send(byte[] message) throws IOException, SocketTimeoutException {
        byte[] buffer = message;
        DatagramPacket packet = new DatagramPacket(buffer, buffer.length, this.servaddr, this.servport);
        this.socket.send(packet);
    }
    
    public byte[] receive() throws IOException, SocketTimeoutException{
        byte[] buffer = new byte[Constants.BUFFER_SIZE];
        DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
        this.socket.receive(packet);
        return Arrays.copyOf(packet.getData(), packet.getLength());
    }
    

}

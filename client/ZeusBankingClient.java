package client; 

import java.net.SocketException;
import java.net.UnknownHostException;

import client.handle.DSBankingHandle;

import client.net.Socket;
import client.net.UDPClient;



public class ZeusBankingClient {

  public static void main(String[] args) throws SocketException, UnknownHostException {
    String host = "localhost";
    int port = 8080;

    for (int i = 0; i < args.length; i++) {
      switch (args[i]) {
          case "--host":
              if (i + 1 < args.length){host = args[++i];}
              break;
          case "--port":
              if (i + 1 < args.length){port = Integer.parseInt(args[++i]);}
              break;
          case "--help":
              System.out.println("Usage: java Server --host <h> --port <p>");
              return;
          default:
              System.out.println("Unknown argument: " + args[i]);
              System.out.println("Usage: java Server --host <h> --port <p>");
              return;
      }
    }

    System.out.println("Connecting to " + host + ":" + port);
    System.out.println("Launching Zeus Banking Client...");
    try {Thread.sleep(1500);} catch (Exception e) {}

    Socket socket = new Socket(host, port);
    UDPClient client = new UDPClient.Builder()
                            .setDebug(false)
                            .setSemanticInvoc(Constants.AT_LEAST_ONE_SEM_INVO)
                            .setTimeoutMs(1000)
                            .setMaxRetry(5)
                            .setSocket(socket)
                            .build();  
    DSBankingHandle dsBankingHandle = new DSBankingHandle(client);
    dsBankingHandle.handle();
    
  }
}

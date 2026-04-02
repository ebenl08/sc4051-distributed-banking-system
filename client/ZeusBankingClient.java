package client; 

// import client.handle.DSBankingHandle;
// import client.net.UDPClient;

import java.net.SocketException;
import java.net.UnknownHostException;

import client.handle.DSBankingHandle;

import client.net.Socket;
import client.net.UDPClient;


// @Command(name = "BankClient", mixinStandardHelpOptions = true, version = "1.0",
//          description = "Connects to the UDP Banking Server.")

public class ZeusBankingClient {

  // @Option(names = {"-h", "--host"}, description = "Server IP address", defaultValue = "127.0.0.1")
  //   private String host;

  // @Option(names = {"-p", "--port"}, description = "Server port", defaultValue = "8080")
  // private int port;

  // @Option(names = {"-t", "--timeout"}, description = "Timeout in ms", defaultValue = "2000")
  // private int timeout;

  // public void run() {
  //     // Your logic starts here, with variables already populated!
  //     System.out.println("Connecting to " + host + ":" + port + " with " + timeout + "ms timeout.");
  //     // launchApp(host, port, timeout);
  // }




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
    
    
    


        // try {
    //   UDPClient communication = new UDPClient();
    //   Socket client = new Socket(communication);


    //   communication.setDebug(true);
    //   communication.setSemanticInvoc(Constants.AT_LEAST_ONE_SEM_INVO);
    //   communication.setTimeoutMs(1000);
    //   communication.setMaxRetry(5);
    //   client.setTimeoutMs(communication.getTimeoutMs());

    //   System.out.println("Communication layer entrypoint is ready.");
    // System.out.println("Client initialized with server " + Constants.DEFAULT_HOST + ":" + Constants.DEFAULT_PORT);
    // } catch (SocketException e) {
    //   System.out.println("Socket error: " + e.getMessage());
    // } catch (UnknownHostException e) {
    //   System.out.println("Host error: " + e.getMessage());
    // }

  }

}

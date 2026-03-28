package client; 

import client.handle.DSBankingHandle;
import client.net.UDPClient;
import java.net.SocketException;
import java.net.UnknownHostException;

public class main {
  public static void main(String[] args) throws SocketException, UnknownHostException {
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

    UDPClient client = new UDPClient();
    DSBankingHandle dsBankingHandle = new DSBankingHandle(client);
    dsBankingHandle.handle();

  }
}

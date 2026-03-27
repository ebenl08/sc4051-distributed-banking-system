package client.handle;


import client.net.UDPClient;
import java.util.Scanner;

public class MonitorHandle {

  private UDPClient udpClient;

  public MonitorHandle(UDPClient udpClient) {
    this.udpClient = udpClient;
  }

  public void handle() {
    UI.clearScreen();
    UI.clearAndHome();

    UI.drawBox("MONITOR");

    UI.clearAndHome();
  }
}
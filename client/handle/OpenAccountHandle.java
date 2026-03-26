package client.handle;

import java.util.Scanner;

import client.net.UDPClient;

public class OpenAccountHandle {

  private UDPClient udpClient;

  public OpenAccountHandle(UDPClient udpClient) {
    this.udpClient = udpClient;
  }

  public void handle() {
    
    UI.clearScreen();
    UI.clearAndHome();

    // INPUT STATE
    UI.drawBox("OPEN ACCOUNT");
    String account = UI.prompt("Enter account name:");
    String password = UI.prompt("Enter account password:");
    String currency = UI.prompt("Enter account currency:");

    
    // CONFIRMATION STATE
    UI.clearScreen();
    UI.drawBox("CONFIRM OPEN ACCOUNT");
    System.out.println("Account: " + account);
    String confirm = UI.prompt("Confirm open account request? (y/n)");
    
    switch (confirm.toLowerCase()) {
        case "y":
          // send request to server to open account
          // code = client.sendRequest(.....)
          break;
        case "n": 
          UI.printSuccess("Open account request cancelled.");
          return;
        default: 
          UI.printError("Invalid input. Please enter 'y' or 'n'.");
          return;
    }
  
    // RESULT STATE
    UI.clearScreen();
    UI.drawBox("OPEN ACCOUNT RESULT");
    if (true) { // replace with actual success condition if(code)
        UI.printSuccess("Account opened successfully.");
    } else {
        UI.printError("Failed to open account. Please try again.");
    }

    UI.clearAndHome();

  }

}
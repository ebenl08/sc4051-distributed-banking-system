package client.handle;

import client.net.UDPClient;

public class CloseAccountHandle {

  private UDPClient client;

  public CloseAccountHandle(UDPClient client) {
    this.client = client;
  }

  public void handle() {
    
    UI.clearScreen();
    UI.clearAndHome();

    // INPUT STATE
    UI.drawBox("CLOSE ACCOUNT");
    String account = UI.prompt("Enter account name:");
    String password = UI.prompt("Enter account password:");
    String currency = UI.prompt("Enter account currency:");

    
    // CONFIRMATION STATE
    UI.clearScreen();
    UI.drawBox("CONFIRM CLOSE ACCOUNT");
    System.out.println("Account: " + account);
    String confirm = UI.prompt("Confirm close account request? (y/n)");
    
    switch (confirm.toLowerCase()) {
        case "y":
          // send request to server to close account
          // code = client.sendRequest(.....)
          break;
        case "n": 
          UI.printSuccess("Close account request cancelled.");
          return;
        default: 
          UI.printError("Invalid input. Please enter 'y' or 'n'.");
          return;
    }
  
    // RESULT STATE
    UI.clearScreen();
    UI.drawBox("CLOSE ACCOUNT RESULT");
    if (true) { // replace with actual success condition if(code)
        UI.printSuccess("Account closed successfully.");
    } else {
        UI.printError("Failed to close account. Please try again.");
    }

    UI.clearAndHome();

  }
}
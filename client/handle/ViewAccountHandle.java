package client.handle;

import client.handle.UI;
import client.net.UDPClient;

public class ViewAccountHandle {

  private UDPClient udpClient;

  public ViewAccountHandle(UDPClient udpClient) {
    this.udpClient = udpClient;
    

  }

  public void handle() {
    
    UI.clearScreen();
    UI.clearAndHome();

    UI.drawBox("VIEW ACCOUNT");
    String account = UI.prompt("Enter account name:");
    String password = UI.prompt("Enter account password:");
    String currency = UI.prompt("Enter account currency:");
    String confirm = UI.prompt("Confirm view account request? (y/n)");
    
    switch (confirm.toLowerCase()) {
        case "y":
          // send request to server to view account
          // code = client.sendRequest(.....)
          break;
        case "n": 
          UI.printSuccess("View account request cancelled.");
          return;
        default: 
          UI.printError("Invalid input. Please enter 'y' or 'n'.");
          return;
    }
    
    // RESULT STATE
    UI.clearScreen();
    UI.drawBox("VIEW ACCOUNT RESULT");
    if (true) { // replace with actual success condition if(code)
        String accountInfo = "Account Name: " + account + "\n" +
                             "Currency: " + currency + "\n" +
                             "Balance: $1000"; // TODO: replace with actual balance from server!!!!
        UI.display(accountInfo);
        UI.prompt("Press Enter to return to main menu...");
    } else {
        UI.printError("Failed to view account.");
    }
    
    
    UI.clearAndHome(); 
    return;
  }

}
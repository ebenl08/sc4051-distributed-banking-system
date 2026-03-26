package client.handle;

import client.handle.UI;
import client.net.UDPClient;

public class TransferMoneyHandle {

  private UDPClient udpClient;

  public TransferMoneyHandle(UDPClient udpClient) {
    this.udpClient = udpClient;
  }

  public void handle() {
    
    UI.clearScreen();
    UI.clearAndHome();

    UI.drawBox("TRANSFER MONEY");
    String fromAccount = UI.prompt("Enter source account number:");
    String toAccount = UI.prompt("Enter destination account number:");
    String amountStr = UI.prompt("Enter amount to transfer:");
    int amount = Integer.parseInt(amountStr);

    UI.clearScreen();
    UI.drawBox("CONFIRM TRANSFER");
    System.out.println("From Account: " + fromAccount);
    System.out.println("To Account: " + toAccount);
    System.out.println("Amount: $" + amount);
    String confirm = UI.prompt("Confirm transfer? (y/n)");

    switch (confirm.toLowerCase()) {
        case "y":
          // send request to server to perform transfer
          // code = client.sendRequest(.....)
          break;
        case "n": 
          UI.printSuccess("Transfer cancelled.");
          return;
        default: 
          UI.printError("Invalid input. Please enter 'y' or 'n'.");
          return;
    }
    
    UI.clearScreen();
    UI.drawBox("TRANSFER RESULT");
    if(true) { // replace with actual success condition if(code)
        UI.printSuccess("Transfer successful.");
    } else {
        UI.printError("Failed to transfer money. Please try again.");
    }
    
    UI.clearAndHome();
    return;
  }
}
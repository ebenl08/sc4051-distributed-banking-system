package client.handle;

import client.handle.UI;
import client.net.UDPClient;

public class DepositWithdrawHandle {

  private UDPClient udpClient;

  public DepositWithdrawHandle(UDPClient udpClient) {
    this.udpClient = udpClient;
  }

  public void handle() {
    
    UI.clearScreen();
    UI.clearAndHome();
    

    // INPUT STATE
    UI.drawBox("DEPOSIT/WITHDRAW");
    String account = UI.prompt("Enter account name:");
    String currency = UI.prompt("Enter account currency:");
    String password = UI.prompt("Enter account password:");
    String amountStr = UI.prompt("Enter amount:");
    UI.drawSeparator();

    int amount = Integer.parseInt(amountStr);
    
    UI.drawSeparator();
    String action = UI.prompt("Deposit or Withdraw? (d/w)");
    String actionstr = switch (action.toLowerCase()) {
        case "d" -> "DEPOSIT";
        case "w" -> "WITHDRAW";
        default -> throw new IllegalArgumentException("Invalid action: " + action);
    };
    

    // CONFIRMATION STATE
    UI.clearScreen();
    UI.drawBox("CONFIRM " + actionstr + "REQUEST?");
    UI.display("Account: " + account);
    UI.display("Currency: " + currency);
    UI.display("Amount: $" + amount);
    String confirm = UI.prompt("Confirm " + actionstr + " request? (y/n)");
    
    // if confirmed, send request to server to perform deposit/withdrawal
    switch (confirm.toLowerCase()) {
        case "y":
          // send request to server to close account
          // code = client.sendRequest(.....)
          break;
        case "n": 
          UI.printSuccess(actionstr + " request cancelled.");
          return;
        default: 
          UI.printError("Invalid input. Please enter 'y' or 'n'.");
          return;
    }

    // RESULT STATE
    UI.clearScreen();
    UI.drawBox(actionstr + " RESULT");
    if (true) { // replace with actual success condition if(code)
        UI.printSuccess(actionstr + " successful.");
    } else {
        UI.printError(actionstr + " failed. Please try again.");
    }    
    
    UI.clearAndHome();

  }

}
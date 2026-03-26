package client.handle;

import client.net.UDPClient;
import java.awt.Adjustable;

public class ChangePasswordHandle{

  private UDPClient client;

  public ChangePasswordHandle(UDPClient client) {
    this.client = client;
  }

  public void handle() {
    
    UI.clearAndHome();

    // INPUT STATE
    UI.clearScreen();
    UI.drawBox("CHANGE PASSWORD");
    String account = UI.prompt("Enter account name:");
    String currency = UI.prompt("Enter account currency:");
    String oldPassword = UI.prompt("Enter old password:");
    String newPassword = UI.prompt("Enter new password:");
    String confirmPassword = UI.prompt("Confirm new password:");
    
    assert newPassword.equals(confirmPassword) : "New password and confirm password do not match.";
    

    // CONFIRMATION STATE
    UI.clearScreen();
    UI.drawBox("CONFIRM CHANGE PASSWORD");
    UI.display("Account: " + account);
    UI.display("Currency: " + currency);
    String confirm = UI.prompt("Confirm change password request? (y/n)");
    
    switch (confirm.toLowerCase()) {
        case "y":
          // send request to server to check old password and change password
          // code = client.sendRequest(.....)
          break;
        case "n": 
          UI.printSuccess("Change password request cancelled.");
          return;
        default: 
          UI.printError("Invalid input. Please enter 'y' or 'n'.");
          return;
    }
    
    // RESULT STATE
    UI.clearScreen();
    UI.drawBox("CHANGE PASSWORD RESULT");
    if (true) { // replace with actual success condition if(code)
        UI.printSuccess("Password changed successfully.");
    } else {
        UI.printError("Failed to change password. Please try again.");
    }
    UI.clearAndHome();
    return;
  }
}
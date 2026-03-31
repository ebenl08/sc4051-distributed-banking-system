package client.handle;


import client.net.UDPClient;

import client.protocol.Response;
import client.protocol.StatusCode;
import client.protocol.Args;

import client.Constants;
import java.io.IOException;
import java.util.concurrent.TimeoutException;

public class ChangePasswordHandle{

  private static final int SERVICE_ID = Constants.SERVICE_CHANGE_PASSWORD;

  private UDPClient client;

  private String account;
  private String name;
  private String oldPassword;
  private String newPassword;

  public ChangePasswordHandle(UDPClient client) {
    this.client = client;
  }

  public void handle() {
    
    this.INPUT_STATE();
    
    if(this.CONFIRMATION_STATE() == 0) {
      this.CLEAN();
      return;
    }

    try{
      Args[] payload = new Args[] {
          Args.ofString(name),
          Args.ofInt32(Integer.parseInt(account)),
          Args.ofString(oldPassword),
          Args.ofString(newPassword),
      };
      
      Response response = this.client.sendRequest(SERVICE_ID, payload);
      this.RESULT_STATE(response);
    } catch (IOException e) {
        UI.printError("[I/O] Error occurred while sending request.");
    } catch (TimeoutException e) {
        UI.printError("[TIMEOUT] Request timed out.");
    } finally {
        this.CLEAN();
        UI.prompt("Press Enter to return to the main menu...");
    }

  }

  private void INPUT_STATE(){
    
    while (true) {
        UI.clearScreen();
        UI.drawBox("CHANGE PASSWORD");
        String account = SafeUI.prompt("Account Number:", SafeUI::validateAccountNumber);
        String name = SafeUI.prompt("Name:", SafeUI::validateName);
        String oldPassword = SafeUI.prompt("Old Password:", SafeUI::validatePassword);
        String newPassword = SafeUI.prompt("New Password:", SafeUI::validatePassword);
        String confirmPassword = SafeUI.prompt("Confirm New Password:",
                  input -> SafeUI.validatePasswordConfirmation(newPassword, input));
        UI.drawSeparator();
            
        this.account = account;
        this.name = name;
        this.oldPassword = oldPassword;
        this.newPassword = newPassword;
        break;
    }
  }

  private int CONFIRMATION_STATE(){
    while(true){
      UI.clearScreen();
      UI.drawBox("CONFIRM CHANGE PASSWORD");
      UI.display("Account: " + this.account);
      UI.display("Name: " + this.name);
      String confirm = UI.prompt("Confirm change password request? (y/n)");
      
      switch (confirm.toLowerCase()) {
          case "y":
            UI.printSuccess("Change password request sent!");
            return 1;
          case "n": 
            UI.display("Change password request cancelled!");
            return 0;
          default: 
            UI.printError("Invalid input. Please enter 'y' or 'n'.");
      }
    }
  }

  private void RESULT_STATE(Response response){
    UI.clearScreen();
    UI.drawBox("CHANGE PASSWORD RESULT");
    if (response.isSuccess()) {
        UI.printSuccess("Password changed successfully.");
    } else {
        int status = response.statusCode;
        String errorMsg = (String) response.payload[0].value;
        switch(status){
            case StatusCode.INVALID_CREDENTIALS:
                UI.printError("INVALID CREDENTIALS: " + errorMsg);
                break;
            case StatusCode.ACCOUNT_NOT_FOUND:
                UI.printError("ACCOUNT NOT FOUND: " + errorMsg);
                break;
            case StatusCode.INTERNAL_ERROR:
            default:
                UI.printError("INTERNAL ERROR: " + errorMsg);
        }
        UI.printError("Failed to change password. Please try again.");
    }
  }

  private void CLEAN(){
    this.account = null;
    this.name = null;
    this.oldPassword = null;
    this.newPassword = null;
  }

}
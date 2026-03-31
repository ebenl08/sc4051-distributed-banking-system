package client.handle;

import client.net.UDPClient;
import client.protocol.Response;
import client.protocol.Args;
import client.Constants;
import java.io.IOException;
import java.util.concurrent.TimeoutException;

public class ViewAccountHandle {

    private static final int SERVICE_ID = Constants.SERVICE_VIEW_ACCOUNT;
    private final UDPClient client;

    // State variables
    private String name;
    private String accountId;
    private String password;

    public ViewAccountHandle(UDPClient udpClient) {
        this.client = udpClient;
    }

    public void handle() {
        try {
            this.INPUT_STATE();

            if (this.CONFIRMATION_STATE() == 0) {
                return;
            }

            // Updated Payload: [string, string, string] -> name, account#, password
            // Note: Ensure your ServiceSchemaFactory matches this new 3-string requirement
            Args[] payload = new Args[]{
                Args.ofString(this.name),
                Args.ofInt32(Integer.parseInt(this.accountId)),
                Args.ofString(this.password)
            };

            Response response = this.client.sendRequest(SERVICE_ID, payload);
            this.RESULT_STATE(response);

        } catch (IOException e) {
            UI.printError("[I/O] Error: " + e.getMessage());
        } catch (TimeoutException e) {
            UI.printError("[TIMEOUT] Server is not responding.");
        } finally {
            this.CLEAN();
        }
    }

    private void INPUT_STATE() {
        UI.clearScreen();
        UI.drawBox("VIEW ACCOUNT");
        this.name = UI.prompt("Enter account name:");
        this.accountId = UI.prompt("Enter account number:");
        this.password = UI.prompt("Enter account password:");
    }

    private int CONFIRMATION_STATE() {
        while (true) {
            UI.clearScreen();
            UI.drawBox("CONFIRM VIEW REQUEST");
            UI.display("Name:       " + this.name);
            UI.display("Account #:  " + this.accountId);
            UI.drawSeparator();
            
            String confirm = UI.prompt("Request balance information? (y/n)");
            switch (confirm.toLowerCase()) {
                case "y":
                    return 1;
                case "n":
                    UI.clearScreen();
                    UI.printSuccess("Request cancelled.");
                    return 0;
                default:
                    UI.printError("Invalid input. Please enter 'y' or 'n'.");
            }
        }
    }

    private void RESULT_STATE(Response response) {
        UI.clearScreen();
        UI.drawBox("ACCOUNT BALANCE RESULT");
        
        if (response.isSuccess()) {
            // Expected return from server: [int64] (balance)
            long balance = (long) response.payload[0].value;
            
            UI.printSuccess("Data retrieved successfully.");
            UI.drawSeparator();
            UI.display("Name: " + this.name);
            UI.display("Account: " + this.accountId);
            UI.display("Current Balance: " + balance);
            UI.drawSeparator();

        } else {
            String errorMsg = (response.payload.length > 0) ? (String) response.payload[0].value : "Authentication failed";
            UI.printError("Failed to view account: " + errorMsg);
        }
        
        UI.prompt("Press Enter to return to main menu...");
    }

    private void CLEAN() {
        this.name = null;
        this.accountId = null;
        this.password = null;
    }
}
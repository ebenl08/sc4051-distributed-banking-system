package client.handle;

import client.Constants;
import client.net.UDPClient;
import client.protocol.Args;
import client.protocol.Response;
import java.io.IOException;
import java.util.concurrent.TimeoutException;

public class OpenAccountHandle {

    private static final int SERVICE_ID = Constants.SERVICE_OPEN_ACCOUNT;
    private final UDPClient client;

    // State variables
    private String name;
    private String password;
    private char currency;
    private long initialBalance;

    public OpenAccountHandle(UDPClient udpClient) {
        this.client = udpClient;
    }

    public void handle() {
        try {
            this.INPUT_STATE();

            if (this.CONFIRMATION_STATE() == 0) {
                return;
            }

            // Schema: [string, string, char, int64]
            Args[] payload = new Args[]{
                Args.ofString(this.name),
                Args.ofString(this.password),
                Args.ofChar(this.currency),
                Args.ofInt64(this.initialBalance)
            };

            Response response = this.client.sendRequest(SERVICE_ID, payload);
            this.RESULT_STATE(response);
        } catch (IOException e) {
            UI.printError("[I/O] Error: " + e.getMessage());
        } catch (TimeoutException e) {
            UI.printError("[TIMEOUT] Server is unavailable.");
        } finally {
            this.CLEAN();
            UI.prompt("Press Enter to return to menu...");
        }
    }

    private void INPUT_STATE() {
        UI.clearScreen();
        UI.drawBox("OPEN NEW ACCOUNT");
        
        this.name = UI.prompt("Enter name:");
        this.password = UI.prompt("Enter password:");
        
        String currInput = UI.prompt("Enter currency (S/U/E):");
        this.currency = currInput.isEmpty() ? 'S' : currInput.toUpperCase().charAt(0);
        
        String balanceStr = UI.prompt("Enter initial balance:");
        this.initialBalance = Long.parseLong(balanceStr);
    }

    private int CONFIRMATION_STATE() {
        while (true) {
            UI.clearScreen();
            UI.drawBox("CONFIRM DETAILS");
            UI.display("Name:     " + this.name);
            UI.display("Currency: " + this.currency);
            UI.display("Balance:  " + this.initialBalance);
            UI.drawSeparator();
            
            String confirm = UI.prompt("Create this account? (y/n)");
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
        UI.drawBox("OPEN ACCOUNT RESULT");
        
        if (response.isSuccess()) {
            // Per your doc, this returns [string] (the new account number)
            String accountNum = (String) response.payload[0].value;
            UI.printSuccess("Account created successfully!");
            UI.display("Your Account Number is: " + accountNum);
        } else {
            String errorMsg = (String) response.payload[0].value;
            UI.printError("Failed to open account: " + errorMsg);
            UI.printError("Please try again with different details"); 
        }
    }
    


    private void CLEAN() {
        this.name = null;
        this.password = null;
        this.currency = ' ';
        this.initialBalance = 0;
    }
}
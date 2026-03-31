package client.handle;

import client.net.UDPClient;
import client.protocol.Response;
import client.protocol.Args;
import client.Constants;

import java.io.IOException;
import java.util.concurrent.TimeoutException;

public class CloseAccountHandle {

    private static final int SERVICE_ID = Constants.SERVICE_CLOSE_ACCOUNT;
    private final UDPClient client;

    // Temporary state variables
    private String name;
    private String account;
    private String password;

    public CloseAccountHandle(UDPClient client) {
        this.client = client;
    }

    public void handle() {
        try {
            this.INPUT_STATE();

            if (this.CONFIRMATION_STATE() == 0) {
                return;
            }

            // [string, string, string] -> name, account#, password
            Args[] payload = new Args[]{
                Args.ofString(this.name),
                Args.ofInt32(Integer.parseInt(this.account)),
                Args.ofString(this.password)
            };

            Response response = this.client.sendRequest(SERVICE_ID, payload);
            this.RESULT_STATE(response);

        } catch (IOException e) {
            UI.printError("[I/O] Error occurred: " + e.getMessage());
        } catch (TimeoutException e) {
            UI.printError("[TIMEOUT] Server did not respond.");
        } finally {
            this.CLEAN();
            UI.prompt("Press Enter to return to menu...");
        }
    }

    private void INPUT_STATE() {
        UI.clearScreen();
        UI.drawBox("CLOSE ACCOUNT");
        this.name = UI.prompt("Enter account name:");
        this.account = UI.prompt("Enter account number:");
        this.password = UI.prompt("Enter account password:");
    }

    private int CONFIRMATION_STATE() {
        while (true) {
            UI.clearScreen();
            UI.drawBox("CONFIRM CLOSE ACCOUNT");
            UI.display("Name:    " + this.name);
            UI.display("Account: " + this.account);
            UI.drawSeparator();
            String confirm = UI.prompt("Close this account PERMANENTLY? (y/n)");

            switch (confirm.toLowerCase()) {
                case "y":
                    return 1;
                case "n":
                    UI.clearScreen();
                    UI.printSuccess("Close account request cancelled.");
                    return 0;
                default:
                    UI.printError("Invalid input. Please enter 'y' or 'n'.");
            }
        }
    }

    private void RESULT_STATE(Response response) {
        UI.clearScreen();
        UI.drawBox("CLOSE ACCOUNT RESULT");
        if (response.isSuccess()) {
            UI.printSuccess("Account closed successfully.");
        } else {
            // Assuming payload[0] contains the error message from server
            String errorMsg = (String) response.payload[0].value;
            UI.printError("Failed to close account: " + errorMsg);
        }
    }

    private void CLEAN() {
        this.name = null;
        this.account = null;
        this.password = null;
    }
}
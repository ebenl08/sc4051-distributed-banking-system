package client.handle;

import client.net.UDPClient;
import client.protocol.Response;
import client.protocol.Args;
import client.Constants;
import java.io.IOException;
import java.util.concurrent.TimeoutException;

public class TransferMoneyHandle {

    private static final int SERVICE_ID = Constants.SERVICE_TRANSFER_MONEY;
    private final UDPClient client;

    // State variables
    private String name1, acc1, name2, acc2, passw1;
    private char curr1, curr2;
    private long amount;

    public TransferMoneyHandle(UDPClient udpClient) {
        this.client = udpClient;
    }

    public void handle() {
        try {
            this.INPUT_STATE();

            if (this.CONFIRMATION_STATE() == 0) {
                return;
            }

            // Schema: [string, string, string, string, char, char, string, int64]
            // (name1, account#1, name2, account#2, currency1, currency2, passw1, amount)
            Args[] payload = new Args[]{
                Args.ofString(name1),
                Args.ofInt32(Integer.parseInt(acc1)),
                Args.ofString(name2),
                Args.ofInt32(Integer.parseInt(acc2)),
                Args.ofChar(curr1),
                Args.ofChar(curr2),
                Args.ofString(this.passw1),
                Args.ofInt64(this.amount)
            };

            Response response = this.client.sendRequest(SERVICE_ID, payload);
            this.RESULT_STATE(response);

        } catch (NumberFormatException e) {
            UI.printError("Invalid amount. Please enter a numeric value.");
        } catch (IOException e) {
            UI.printError("[I/O] Network error: " + e.getMessage());
        } catch (TimeoutException e) {
            UI.printError("[TIMEOUT] Server is not responding.");
        } finally {
            this.CLEAN();
            UI.prompt("Press Enter to return to menu...");
        }
    }

    private void INPUT_STATE() {
        UI.clearScreen();
        UI.drawBox("TRANSFER MONEY");
        
        // Sender Info
        this.name1 = UI.prompt("Your Name:");
        this.acc1  = UI.prompt("Your Account #:");
        this.passw1 = UI.prompt("Your Password:");
        this.curr1 = UI.prompt("Your Currency (S/U/E):").toUpperCase().charAt(0);
        
        UI.drawSeparator();
        
        // Receiver Info
        this.name2 = UI.prompt("Receiver Name:");
        this.acc2  = UI.prompt("Receiver Account #:");
        this.curr2 = UI.prompt("Receiver Currency (S/U/E):").toUpperCase().charAt(0);
        
        UI.drawSeparator();
        this.amount = Long.parseLong(UI.prompt("Amount to Transfer:"));
    }

    private int CONFIRMATION_STATE() {
        while (true) {
            UI.clearScreen();
            UI.drawBox("CONFIRM TRANSFER");
            UI.display("FROM: " + this.name1 + " (" + this.acc1 + ")");
            UI.display("TO:   " + this.name2 + " (" + this.acc2 + ")");
            UI.display("AMT:  " + this.amount + " " + this.curr1 + " -> " + this.curr2);
            UI.drawSeparator();
            
            String confirm = UI.prompt("Proceed with transfer? (y/n)");
            switch (confirm.toLowerCase()) {
                case "y": return 1;
                case "n": 
                    UI.clearScreen();
                    UI.printSuccess("Transfer cancelled.");
                    return 0;
                default: UI.printError("Please enter 'y' or 'n'.");
            }
        }
    }

    private void RESULT_STATE(Response response) {
        UI.clearScreen();
        UI.drawBox("TRANSFER RESULT");
        
        if (response.isSuccess()) {
            UI.printSuccess("Transfer Successful!");
            // Doc Output: [string, int32, int64, string] -> (name2, acc2, amount, msg)
            if (response.payload.length >= 4) {
                UI.display("Recipient: " + response.payload[0].value);
                UI.display("Final Amount: " + response.payload[2].value);
                UI.display("Message: " + response.payload[3].value);
            }
        } else {
            String error = (response.payload.length > 0) ? (String) response.payload[0].value : "Unknown error";
            UI.printError("Transfer Failed: " + error);
        }
    }

    private void CLEAN() {
        this.name1 = this.acc1 = this.name2 = this.acc2 = this.passw1 = null;
        this.amount = 0;
    }
}
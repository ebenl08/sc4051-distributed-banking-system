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
    private String curr1, curr2;
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
                Args.ofChar(SafeUI.currencyCodeToChar(curr1)),
                Args.ofChar(SafeUI.currencyCodeToChar(curr2)),
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
            UI.drawSeparator();
            UI.prompt("Press Enter to return to menu...");
        }
    }

    private void INPUT_STATE() {
        UI.clearScreen();
        UI.drawBox("TRANSFER MONEY");

        // Sender Info
        String name1    = SafeUI.prompt("Sender Name            :", SafeUI::validateName);
        String acc1     = SafeUI.prompt("Sender Account Number  :", SafeUI::validateAccountNumber);
        String passw1   = SafeUI.prompt("Sender Password        :", SafeUI::validatePassword);
        String curr1    = SafeUI.prompt("Sender Currency (SGD/USD/EUR):", SafeUI::validateCurrency);
        
        this.name1 = name1;
        this.acc1  = acc1;
        this.passw1 = passw1;
        this.curr1 = curr1.toUpperCase();

        UI.drawSeparator();
        UI.display("Receiver Information");
        UI.drawSeparator();

        // Receiver Info
        String name2 = SafeUI.prompt("Name              :", SafeUI::validateName);
        String acc2 = SafeUI.prompt("Account Number    :", SafeUI::validateAccountNumber);
        String curr2 = SafeUI.prompt("Currency (SGD/USD/EUR):", SafeUI::validateCurrency);
        
        this.name2 = name2;
        this.acc2  = acc2;
        this.curr2 = curr2.toUpperCase();

        UI.drawSeparator();
        String amountStr = SafeUI.prompt("Amount to Transfer:", SafeUI::validateMoneyInput);
        this.amount = SafeUI.validateMoneyInput(amountStr);
    }

    private int CONFIRMATION_STATE() {
        while (true) {
            UI.clearScreen();
            UI.drawBox("CONFIRM TRANSFER");
            UI.display("FROM: " + this.name1 + " (" + this.acc1 + ")");
            UI.display("TO  : " + this.name2 + " (" + this.acc2 + ")");
            UI.display("AMT : " + String.format("%.2f", this.amount / 100.0) + " " + 
                    this.curr1 + " -> " + this.curr2);
            UI.drawSeparator();
            
            String confirm = SafeUI.prompt("Confirm transfer? (y/n)", SafeUI::validateYesNo);
            switch (confirm.toLowerCase()) { 
                case "y":
                    return 1;
                case "n":
                    UI.clearScreen();
                    UI.printSuccess("Request cancelled.");
                    return 0;
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
                float amountTransferredFloat = (long) amount * (float) response.payload[3].value;
                long amountTransferred = Math.round(amountTransferredFloat);

                UI.display("Recipient           : " + response.payload[0].value);
                UI.display("Recipient Acc#      : " + response.payload[1].value);
                UI.display("Exchange Rate       : " + String.format("%.2f", (response.payload[3].value)));
                UI.display("Amount Transferred  : " + curr2 + " " + SafeUI.moneyToString(amountTransferred));
                UI.display("Remaining Balance   : " + curr1 + " " + SafeUI.moneyToString((long) response.payload[4].value));
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
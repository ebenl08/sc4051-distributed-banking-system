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
    private String currency;
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
                Args.ofChar(SafeUI.currencyCodeToChar(this.currency)),
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
            UI.drawSeparator();
            UI.prompt("Press Enter to return to menu...");
        }
    }

    private void INPUT_STATE() {
        UI.clearScreen();
        UI.drawBox("OPEN NEW ACCOUNT");
        
        String name = SafeUI.prompt("Name            :", SafeUI::validateName);
        String password = SafeUI.prompt("Password        :", SafeUI::validatePassword);
        String currency = SafeUI.prompt("Currency (SGD/USD/EUR):", SafeUI::validateCurrency);
        String balance = SafeUI.prompt("Initial Balance :", SafeUI::validateMoneyInput);

        this.name = name;
        this.password = password;
        this.currency = currency.toUpperCase();
        this.initialBalance = SafeUI.validateMoneyInput(balance);
    }

    private int CONFIRMATION_STATE() {
        while (true) {
            UI.clearScreen();
            UI.drawBox("CONFIRM DETAILS");
            UI.display("Name        : " + this.name);
            UI.display("Currency    : " + this.currency);
            UI.display("Balance     : " + String.format("%.2f", this.initialBalance / 100.0));
            UI.drawSeparator();
            
            String confirm = SafeUI.prompt("Create this account? (y/n)", SafeUI::validateYesNo);
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
        UI.drawBox("OPEN ACCOUNT RESULT");
        
        if (response.isSuccess()) {
            // Per your doc, this returns [string] (the new account number)
            int accountNum = (int) response.payload[0].value;
            UI.printSuccess("Account created successfully!");
            UI.drawSeparator();
            UI.display("Your Account Number is: " + accountNum);
            UI.display("Please keep it safe for future transactions.");
            UI.drawSeparator();

        } else {
            String errorMsg = (String) response.payload[0].value;
            UI.printError("Failed to open account: " + errorMsg);
            UI.printError("Please try again with different details"); 
        }
    }
    


    private void CLEAN() {
        this.name = null;
        this.password = null;
        this.currency = null;
        this.initialBalance = 0;
    }
}
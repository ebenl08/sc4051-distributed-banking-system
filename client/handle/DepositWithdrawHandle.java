package client.handle;

import client.net.UDPClient;
import client.protocol.Response;
import client.protocol.Args;
import client.Constants;
import java.io.IOException;
import java.util.concurrent.TimeoutException;

public class DepositWithdrawHandle {

    private static final int SERVICE_ID = Constants.SERVICE_DEPOSIT_WITHDRAW_MONEY;
    private final UDPClient client;

    private String name;
    private String account;
    private String password;
    private String currency;
    private String actionStr; // "DEPOSIT" or "WITHDRAWAL"
    private long amount;

    public DepositWithdrawHandle(UDPClient udpClient) {
        this.client = udpClient;
    }

    public void handle() {
        try {
            this.INPUT_STATE();

            if (this.CONFIRMATION_STATE() == 0) {
                return;
            }

            Args[] payload = new Args[]{
                Args.ofString(name),
                Args.ofInt32(Integer.parseInt(account)),
                Args.ofString(password),
                Args.ofChar(currency.charAt(0)),
                Args.ofInt64(amount)
            };

            Response response = this.client.sendRequest(SERVICE_ID, payload);
            this.RESULT_STATE(response);

        } catch (IllegalArgumentException e) {
            UI.printError(e.getMessage());
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
        UI.drawBox("DEPOSIT/WITHDRAW");
        
        String name = SafeUI.prompt("Name            :", SafeUI::validateName);
        String account = SafeUI.prompt("Account Number  :", SafeUI::validateAccountNumber);
        String password = SafeUI.prompt("Password        :", SafeUI::validatePassword);
        String currency = SafeUI.prompt("Currency (SGD/USD/EUR):", SafeUI::validateCurrency);

        this.name = name;
        this.account = account;
        this.password = password;
        this.currency = currency.toUpperCase();
        
        String amountStr = SafeUI.prompt("Enter amount:", SafeUI::validateMoneyInput);
        this.amount = SafeUI.validateMoneyInput(amountStr);

        UI.drawSeparator();
        String action = SafeUI.prompt("Deposit or Withdraw? (d/w)", SafeUI::validateDepositWithdraw);

        if (action.toLowerCase().equals("w")) {
            this.actionStr = "WITHDRAWAL";
            this.amount = -this.amount; // Negate amount for withdrawal
        }else{
            this.actionStr = "DEPOSIT"; 
        }
    }

    private int CONFIRMATION_STATE() {
        while (true) {
            UI.clearScreen();
            UI.drawBox("CONFIRM " + this.actionStr);
            UI.display("Account     : " + this.account);
            UI.display("Currency    : " + this.currency);
            UI.display("Amount      : " + String.format("%.2f", this.amount / 100.0));
            UI.drawSeparator();
            
            String confirm = UI.prompt("Confirm " + this.actionStr + " request? (y/n)");
            switch (confirm.toLowerCase()) {
                case "y":
                    return 1;
                case "n":
                    UI.clearScreen();
                    UI.printSuccess(this.actionStr + " cancelled.");
                    return 0;
                default:
                    UI.printError("Invalid input. Please enter 'y' or 'n'.");
            }
        }
    }

    private void RESULT_STATE(Response response) {
        UI.clearScreen();
        UI.drawBox(this.actionStr + " RESULT");
        
        if (response.isSuccess()) {
            UI.printSuccess(this.actionStr + " successful.");
            // Your doc says this returns [string, int64] (msg, new balance)
            UI.drawSeparator();
            UI.display("New Balance   : " + SafeUI.moneyToString((long) response.payload[0].value));
        } else {
            String errorMsg = (response.payload.length > 0) ? (String) response.payload[0].value : "Unknown error";
            UI.printError(this.actionStr + " failed: " + errorMsg);
        }
    }

    private void CLEAN() {
        this.name = null;
        this.account = null;
        this.password = null;
        this.currency = null;
        this.amount = 0;
        this.actionStr = null;
    }
}
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
    private long amount;
    private String actionStr;

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
                new Args(Args.Type.STRING, this.name),
                new Args(Args.Type.STRING, this.account),
                new Args(Args.Type.STRING, this.password),
                new Args(Args.Type.CHAR, this.currency.charAt(0)),
                new Args(Args.Type.INT64, this.amount)
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
            UI.prompt("Press Enter to return to menu...");
        }
    }

    private void INPUT_STATE() {
        UI.clearScreen();
        UI.drawBox("DEPOSIT/WITHDRAW");
        
        this.name = UI.prompt("Enter name:");
        this.account = UI.prompt("Enter account number:");
        this.password = UI.prompt("Enter password:");
        this.currency = UI.prompt("Enter currency (e.g., S, U, E):");
        
        String amountStr = UI.prompt("Enter amount:");
        this.amount = Long.parseLong(amountStr);

        UI.drawSeparator();
        String action = UI.prompt("Deposit or Withdraw? (d/w)");
        this.actionStr = switch (action.toLowerCase()) {
            case "d" -> "DEPOSIT";
            case "w" -> "WITHDRAW";
            default -> throw new IllegalArgumentException("Invalid action: " + action);
        };
    }

    private int CONFIRMATION_STATE() {
        while (true) {
            UI.clearScreen();
            UI.drawBox("CONFIRM " + this.actionStr);
            UI.display("Account:  " + this.account);
            UI.display("Currency: " + this.currency);
            UI.display("Amount:   " + this.amount);
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
            if (response.payload.length > 1) {
                UI.display("New Balance: " + response.payload[1].value);
            }
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
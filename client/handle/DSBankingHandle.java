package client.handle;

import java.util.Scanner;

import client.net.UDPClient;

public class DSBankingHandle {
    
    // Handler instances
    private UDPClient client;

    private static OpenAccountHandle openAccountHandle;
    private static CloseAccountHandle closeAccountHandle;
    private static DepositWithdrawHandle depositWithdrawHandle;
    private static TransferMoneyHandle transferMoneyHandle;
    private static ViewAccountHandle viewAccountHandle;
    private static MonitorHandle monitorHandle;
    private static ChangePasswordHandle changePasswordHandle;

    public DSBankingHandle(UDPClient client) {
        this.client = client;
        openAccountHandle = new OpenAccountHandle(client);
        closeAccountHandle = new CloseAccountHandle(client);
        depositWithdrawHandle = new DepositWithdrawHandle(client);
        transferMoneyHandle = new TransferMoneyHandle(client);
        viewAccountHandle = new ViewAccountHandle(client);
        monitorHandle = new MonitorHandle(client);
        changePasswordHandle = new ChangePasswordHandle(client);
    }

    public void handle(){
        Runtime.getRuntime().addShutdownHook(new Thread(UI::showCursor));
        UI.hideCursor();

        while (true) {
            UI.clearScreen();
            UI.drawBox("ZEUS BANKING SYSTEM");

            System.out.println("  1. Open Account         5. View Account");
            System.out.println("  2. Close Account        6. Monitor All");
            System.out.println("  3. Deposit/Withdraw     7. Change Password");
            System.out.println("  4. Transfer Money       8. Exit");
            UI.drawSeparator();

            String choice = SafeUI.prompt("SELECT OPTION (1-8)", SafeUI::validateNumber);
            int choiceNum = Integer.parseInt(choice);
            switch (choiceNum) {
                case 1 -> openAccountHandle.handle();
                case 2 -> closeAccountHandle.handle();
                case 3 -> depositWithdrawHandle.handle();
                case 4 -> transferMoneyHandle.handle();
                case 5 -> viewAccountHandle.handle();
                case 6 -> monitorHandle.handle();
                case 7 -> changePasswordHandle.handle();
                case 8 -> {
                    UI.drawSeparator();
                    UI.printSuccess("Thank you for using Zeus Banking System!");
                    return;
                }
                default -> {
                    UI.drawSeparator();
                    System.out.println(UI.RED + "Invalid option. Please try again." + UI.RESET);
                    UI.sleep();
                }
            }

        }
    }
}


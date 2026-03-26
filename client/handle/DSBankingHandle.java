package client.handle;

import java.util.Scanner;
import client.handle.UI;
import client.net.Socket;

public class DSBankingHandle {

    private static final Scanner scanner = new Scanner(System.in);
    
    public static void main(String[] args) {
        Runtime.getRuntime().addShutdownHook(new Thread(UI::showCursor));
        UI.hideCursor();
        UI.clearAndHome();

        while (true) {
            UI.clearScreen();
            renderHeader("ZEUS BANKING SYSTEM");

            System.out.println("  1. Open Account         5. View Account");
            System.out.println("  2. Close Account        6. Monitor All");
            System.out.println("  3. Deposit/Withdraw     7. Change Password");
            System.out.println("  4. Transfer Money       8. Exit");
            UI.drawSeparator();

            String choice = UI.prompt("SELECT OPTION (1-8)");

            if ("4".equals(choice)) {
                // handleTransferPage(); // Uncomment and implement if needed
            } else if ("8".equals(choice)) {
                break;
            }
        }
    }



    public static void handleMonitorPage() {
        UI.clearScreen();
        renderHeader("MONITOR SERVICE");

        // TODO: Implement monitor service

    }

    public static void renderHeader(String title){
        UI.clearAndHome();
        UI.drawBox(title);
    }
}


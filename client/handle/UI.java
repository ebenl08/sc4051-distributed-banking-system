package client.handle;

import java.util.Scanner;

public class UI {

    // Constants
    public static final String HIDE_CURSOR = "\033[?25l";
    public static final String SHOW_CURSOR = "\033[?25h";

    public static final String CURSOR_UP = "\033[F";
    public static final String CURSOR_BEGIN = "\r";
    public static final String CLEAR_LINE = "\033[2K";
    
    public static final String HOME  = "\033[H";
    public static final String RESET = "\033[0m";

    public static final String CYAN  = "\033[0;36m";
    public static final String GREEN = "\033[0;32m";
    public static final String BRIGHT_CYAN   = "\033[1;36m";
    public static final String RED    = "\033[0;31m";
    public static final String YELLOW = "\033[0;33m";
    public static final String BOLD   = "\033[1m";
    

    private static final String BOX_TOP   = "╔══════════════════════════════════════════════════════╗";
    private static final String BOX_BOTTOM = "╚══════════════════════════════════════════════════════╝";
    private static final String SEPARATOR = "──────────────────────────────────────────────────────────";


    public static void hideCursor()   { System.out.print(HIDE_CURSOR); }
    public static void showCursor()   { System.out.print(SHOW_CURSOR); }
    public static void clearScreen() {
        System.out.print("\033[2J\033[3J" + HOME);
        System.out.flush();
    }

    public static void sleep(int ms) {
        try { Thread.sleep(ms); } catch (Exception e) {}
    }
    public static void sleep() {
        try { Thread.sleep(1500); } catch (Exception e) {}
    }

    public static void drawBox(String title) {
        System.out.println(BOLD + BRIGHT_CYAN +BOX_TOP);
        System.out.printf("║ %-52s ║\n", centerText(title, 52));
        System.out.println(BOX_BOTTOM + RESET);
        UI.display("");
    }
    public static void drawSeparator() { System.out.println(BOLD + SEPARATOR + RESET); }

    
    public static String prompt(String message) {
        System.out.print(message + "> ");
        Scanner s = new Scanner(System.in);
        return s.nextLine();
    }

    public static void printSuccess(String msg) {
        System.out.println(GREEN + "[SUCCESS] " + msg + RESET);
        UI.sleep();
    }
    public static void printError(String msg) {
        System.out.println(RED + "[ERROR] " + msg + RESET);
        UI.sleep();
    }
    public static void printWarning(String msg) {
        System.out.println(YELLOW + "[WARNING] " + msg + RESET);
        UI.sleep();
    }


    public static void display(String msg){
        System.out.println(msg);
    }

    public static void resetLine() {
        // Move cursor to beginning of line and clear it
        System.out.print(CURSOR_BEGIN + CLEAR_LINE);
        System.out.flush();
    }

    public static void cursorUp() {
        System.out.print(CURSOR_UP);
        System.out.flush();
    }

    private static String centerText(String text, int width) {
        int pad = (width - text.length()) / 2;
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < pad; i++) sb.append(" ");
        sb.append(text);
        while (sb.length() < width) sb.append(" ");
        return sb.toString();
    }
}


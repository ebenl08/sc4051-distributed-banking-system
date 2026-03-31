import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.*;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;
import java.util.Scanner;

public class UDPTestClient {

    private static final String HOST = "127.0.0.1";
    private static final int PORT = 8080;
    private static final int BUFFER_SIZE = 1500;

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int reqID = 1;

        while (true) {
            try {
                System.out.println("\n=== Select Service ===");
                System.out.println("1. Open Account");
                System.out.println("2. Close Account");
                System.out.println("3. Deposit/Withdraw");
                System.out.println("4. Transfer");
                System.out.println("5. View Account");
                System.out.println("6. Monitor");
                System.out.println("7. Change Password");
                System.out.println("0. Exit");

                int choice = scanner.nextInt();
                scanner.nextLine();

                if (choice == 0) break;

                byte[] request = null;

                switch (choice) {
                    case 1: {
                        System.out.print("Username: ");
                        String username = scanner.nextLine();

                        System.out.print("Password: ");
                        String password = scanner.nextLine();

                        System.out.print("Currency (S/U/E): ");
                        char currency = scanner.nextLine().charAt(0);

                        System.out.print("Initial Balance: ");
                        long balance = scanner.nextLong();
                        scanner.nextLine();

                        request = buildService1Request(reqID++, username, password, currency, balance);
                        break;
                    }

                    case 2: {
                        System.out.print("Username: ");
                        String username = scanner.nextLine();

                        System.out.print("Account Number: ");
                        int acc = scanner.nextInt();
                        scanner.nextLine();

                        System.out.print("Password: ");
                        String password = scanner.nextLine();

                        request = buildService2Request(reqID++, username, acc, password);
                        break;
                    }

                    case 3: {
                        System.out.print("Username: ");
                        String username = scanner.nextLine();

                        System.out.print("Account Number: ");
                        int acc = scanner.nextInt();
                        scanner.nextLine();

                        System.out.print("Password: ");
                        String password = scanner.nextLine();

                        System.out.print("Action (deposit/withdraw): ");
                        String action = scanner.nextLine();

                        System.out.print("Amount: ");
                        long amount = scanner.nextLong();
                        scanner.nextLine();

                        request = buildService3Request(reqID++, username, acc, password, action, amount);
                        break;
                    }

                    case 4: {
                        System.out.print("Sender Username: ");
                        String sender = scanner.nextLine();

                        System.out.print("From Account: ");
                        int fromAcc = scanner.nextInt();
                        scanner.nextLine();

                        System.out.print("Password: ");
                        String password = scanner.nextLine();

                        System.out.print("To Account: ");
                        int toAcc = scanner.nextInt();
                        scanner.nextLine();

                        System.out.print("From Currency: ");
                        char fromCur = scanner.nextLine().charAt(0);

                        System.out.print("To Currency: ");
                        char toCur = scanner.nextLine().charAt(0);

                        System.out.print("Recipient Username: ");
                        String recipient = scanner.nextLine();

                        System.out.print("Amount: ");
                        long amount = scanner.nextLong();
                        scanner.nextLine();

                        request = buildService4Request(reqID++, sender, fromAcc, password, toAcc, fromCur, toCur, recipient, amount);
                        break;
                    }

                    case 5: {
                        System.out.print("Username: ");
                        String username = scanner.nextLine();

                        System.out.print("Account Number: ");
                        int acc = scanner.nextInt();
                        scanner.nextLine();

                        System.out.print("Password: ");
                        String password = scanner.nextLine();

                        request = buildService5Request(reqID++, username, acc, password);
                        break;
                    }

                    case 6: {
                        System.out.print("Monitor Interval (seconds): ");
                        int interval = scanner.nextInt();
                        scanner.nextLine();

                        request = buildService6Request(reqID++, interval);
                        break;
                    }

                    case 7: {
                        System.out.print("Username: ");
                        String username = scanner.nextLine();

                        System.out.print("Account Number: ");
                        int acc = scanner.nextInt();
                        scanner.nextLine();

                        System.out.print("Password: ");
                        String password = scanner.nextLine();

                        System.out.print("New Password: ");
                        String newPass = scanner.nextLine();

                        request = buildService7Request(reqID++, username, acc, password, newPass);
                        break;
                    }

                    default:
                        System.out.println("Invalid choice!");
                        continue;
                }

                sendAndReceive(request);

            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        scanner.close();
    }

    private static void sendAndReceive(byte[] request) throws IOException {
        DatagramSocket socket = new DatagramSocket();
        socket.setSoTimeout(5000);

        InetAddress serverAddress = InetAddress.getByName(HOST);

        DatagramPacket packet = new DatagramPacket(
                request,
                request.length,
                serverAddress,
                PORT
        );

        socket.send(packet);
        System.out.println("Request sent.");

        byte[] buffer = new byte[BUFFER_SIZE];
        DatagramPacket responsePacket = new DatagramPacket(buffer, buffer.length);

        socket.receive(responsePacket);
        System.out.println("Response received.");

        parseResponse(responsePacket.getData(), responsePacket.getLength());

        socket.close();
    }

    private static String readMarshalledString(ByteBuffer buffer) {
        int strLength = buffer.getInt();

        if (strLength < 0 || strLength > buffer.remaining()) {
            throw new IllegalArgumentException("Invalid string length: " + strLength);
        }

        byte[] strBytes = new byte[strLength];
        buffer.get(strBytes);

        return new String(strBytes, StandardCharsets.UTF_8);
    }

    private static void parseResponse(byte[] data, int length) {
        ByteBuffer buffer = ByteBuffer.wrap(data, 0, length);
        buffer.order(ByteOrder.BIG_ENDIAN);

        int reqID = buffer.getInt();
        int serviceID = buffer.getInt();
        int statusCode = buffer.getInt();

        System.out.println("=== Response ===");
        System.out.println("reqID      : " + reqID);
        System.out.println("serviceID  : " + serviceID);
        System.out.println("statusCode : " + statusCode);

        if (length <= 12) {
            System.out.println("No payload.");
            return;
        }

        try {
            String payloadMessage = readMarshalledString(buffer);

            System.out.println("Message: " + payloadMessage);

            if (serviceID == 1 && statusCode == 0) {
                int accountNumber = Integer.parseInt(payloadMessage);
                System.out.println(">>> Account Number = " + accountNumber);
            }
        } catch (Exception e) {
            System.out.println("Failed to parse payload: " + e.getMessage());
        }
    }

    // =========================
    // SERVICE BUILDERS
    // =========================

    public static byte[] buildService1Request(int reqID, String username, String password, char currency, long initialBalance)
            throws IOException {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        DataOutputStream out = new DataOutputStream(baos);

        out.writeInt(reqID);
        out.writeInt(1);
        out.writeInt(0);

        writeString(out, username);
        writeString(out, password);
        out.writeByte((byte) currency);
        out.writeLong(initialBalance);

        return baos.toByteArray();
    }

    public static byte[] buildService2Request(int reqID, String username, int accountNumber, String password)
            throws IOException {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        DataOutputStream out = new DataOutputStream(baos);

        out.writeInt(reqID);
        out.writeInt(2);
        out.writeInt(0);

        writeString(out, username);
        out.writeInt(accountNumber);
        writeString(out, password);

        return baos.toByteArray();
    }

    public static byte[] buildService3Request(int reqID, String username, int accountNumber, String password, String action, long amount)
            throws IOException {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        DataOutputStream out = new DataOutputStream(baos);

        out.writeInt(reqID);
        out.writeInt(3);
        out.writeInt(0);

        writeString(out, username);
        out.writeInt(accountNumber);
        writeString(out, password);
        writeString(out, action);
        out.writeLong(amount);

        return baos.toByteArray();
    }

    public static byte[] buildService4Request(int reqID, String senderUsername, int fromAccount, String password,
                                             int toAccount, char fromCurrency, char toCurrency,
                                             String recipientUsername, long amount) throws IOException {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        DataOutputStream out = new DataOutputStream(baos);

        out.writeInt(reqID);
        out.writeInt(4);
        out.writeInt(0);

        writeString(out, senderUsername);
        out.writeInt(fromAccount);
        writeString(out, password);
        out.writeInt(toAccount);
        out.writeByte((byte) fromCurrency);
        out.writeByte((byte) toCurrency);
        writeString(out, recipientUsername);
        out.writeLong(amount);

        return baos.toByteArray();
    }

    public static byte[] buildService5Request(int reqID, String username, int accountNumber, String password)
            throws IOException {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        DataOutputStream out = new DataOutputStream(baos);

        out.writeInt(reqID);
        out.writeInt(5);
        out.writeInt(0);

        writeString(out, username);
        out.writeInt(accountNumber);
        writeString(out, password);

        return baos.toByteArray();
    }

    public static byte[] buildService6Request(int reqID, int intervalSeconds) throws IOException {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        DataOutputStream out = new DataOutputStream(baos);

        out.writeInt(reqID);
        out.writeInt(6);
        out.writeInt(0);

        out.writeInt(intervalSeconds);

        return baos.toByteArray();
    }

    public static byte[] buildService7Request(int reqID, String username, int accountNumber, String password, String newPassword)
            throws IOException {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        DataOutputStream out = new DataOutputStream(baos);

        out.writeInt(reqID);
        out.writeInt(7);
        out.writeInt(0);

        writeString(out, username);
        out.writeInt(accountNumber);
        writeString(out, password);
        writeString(out, newPassword);

        return baos.toByteArray();
    }

    private static void writeString(DataOutputStream out, String value) throws IOException {
        byte[] bytes = value.getBytes(StandardCharsets.UTF_8);
        out.writeInt(bytes.length);
        out.write(bytes);
    }
}
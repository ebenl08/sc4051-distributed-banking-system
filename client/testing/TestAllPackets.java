package client.testing;

import client.protocol.Request;
import client.protocol.Response;
import client.net.UDPClient;
import client.handle.UI;

import java.util.List;

public class TestAllPackets {
    public static void main(String[] args) {
        UDPClient client = null;
        try {
            client = new UDPClient();
        } catch (Exception e) {
            e.printStackTrace();
            return;
        }

        List<Request> requests = AllRequestPackets.getAllRequests();
        int idx = 1;
        for (Request req : requests) {
            System.out.println("\n--- Test Packet #" + idx + " ---");
            System.out.println("Request ID: " + req.requestId);
            System.out.println("Service ID: " + req.serviceId);
            System.out.println("Payload:");
            for (var arg : req.payload) {
                System.out.println("  " + arg);
            }
            try {
                Response response = client.sendRequest(req.serviceId, req.payload);
                System.out.println("Response:");
                System.out.println("  Response ID: " + response.responseId);
                System.out.println("  Status Code: " + response.statusCode);
                System.out.println("Payload:");
                for (var arg : response.payload) {
                    System.out.println("  " + arg);
                }
            } catch (Exception e) {
                System.out.println("  [ERROR] Exception: " + e.getMessage());
                System.out.println("  [ERROR] Stack Trace:");
                e.printStackTrace();
            }
            idx++;
            try {
                Thread.sleep(2000); // Sleep briefly between tests for readability
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            UI.prompt("Press Enter to continue to next packet test...");
        }
    }
}

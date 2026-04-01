package client.testing;

import client.Utils;

import client.protocol.Request;
import client.protocol.Response;
import client.protocol.Args;

import client.net.PacketUtils;
import client.net.UDPClient;
import com.sun.jdi.connect.Connector;

public class TestPacket {

    public static void testRequestPacket(UDPClient client) {
        
        // Construct a sample request
        Request request = new Request(
            12345, 
            7, 
            new Args[] {
                Args.ofString("testUser"),
                Args.ofInt32(1000),
                Args.ofString("oldPass"),
                Args.ofString("newPass")
            }
        );


        try{
            Response response = client.sendRequest(7, request.payload);

            System.out.println("Received response:");
            System.out.println("Request ID: " + response.responseId);
            System.out.println("Status Code: " + response.statusCode);
            for (Args arg : response.payload) {
                System.out.println(arg.toString());
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

    }

    public static void main(String[] args) {
        // Example usage of PacketBuilder and Marshalling
        // This is just a placeholder for your actual test cases
        System.out.println("Testing packet construction and parsing...");
        
        // You can create a UDPClient instance and call testRequestPacket to see the outpu
        UDPClient client =null;
        try {
            client = new UDPClient();
        } catch (Exception e) {
            e.printStackTrace();
        }

        testRequestPacket(client);
    }
}

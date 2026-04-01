package client.testing;

import client.protocol.Args;
import client.protocol.Request;
import client.Constants;

import java.util.ArrayList;
import java.util.List;

public class AllRequestPackets {
    public static List<Request> getAllRequests() {
        List<Request> requests = new ArrayList<>();

        // 1. Alice creates account
        requests.add(new Request(1, Constants.SERVICE_OPEN_ACCOUNT, new Args[]{
            Args.ofString("Alice"),
            Args.ofString("alicepass"),
            Args.ofChar((char)Constants.SGD),
            Args.ofInt64(10000L)
        }));
        // 2. Bob creates account
        requests.add(new Request(2, Constants.SERVICE_OPEN_ACCOUNT, new Args[]{
            Args.ofString("Bob"),
            Args.ofString("bobpass"),
            Args.ofChar((char)Constants.USD),
            Args.ofInt64(5000L)
        }));
        // 3. Charlie creates account
        requests.add(new Request(3, Constants.SERVICE_OPEN_ACCOUNT, new Args[]{
            Args.ofString("Charlie"),
            Args.ofString("charliepass"),
            Args.ofChar((char)Constants.EUR),
            Args.ofInt64(2000L)
        }));
        // 4. Charlie closes account
        requests.add(new Request(4, Constants.SERVICE_CLOSE_ACCOUNT, new Args[]{
            Args.ofString("Charlie"),
            Args.ofInt32(2),
            Args.ofString("charliepass")
        }));
        // 5. Alice deposits money
        requests.add(new Request(5, Constants.SERVICE_DEPOSIT_WITHDRAW_MONEY, new Args[]{
            Args.ofString("Alice"),
            Args.ofInt32(0), // assuming account# 1001
            Args.ofString("alicepass"),
            Args.ofChar((char)Constants.SGD),
            Args.ofInt64(3000L)
        }));
        // 6. Bob withdraws money
        requests.add(new Request(6, Constants.SERVICE_DEPOSIT_WITHDRAW_MONEY, new Args[]{
            Args.ofString("Bob"),
            Args.ofInt32(1), // assuming account# 1002
            Args.ofString("bobpass"),
            Args.ofChar((char)Constants.USD),
            Args.ofInt64(-2000L)
        }));
        // 7. Alice transfers money to Bob
        requests.add(new Request(7, Constants.SERVICE_TRANSFER_MONEY, new Args[]{
            Args.ofString("Alice"),
            Args.ofInt32(0),
            Args.ofString("Bob"),
            Args.ofInt32(1),
            Args.ofChar((char)Constants.SGD),
            Args.ofChar((char)Constants.USD),
            Args.ofString("alicepass"),
            Args.ofInt64(1500L)
        }));
        // 8. Bob changes password
        requests.add(new Request(8, Constants.SERVICE_CHANGE_PASSWORD, new Args[]{
            Args.ofString("Bob"),
            Args.ofInt32(1),
            Args.ofString("bobpass"),
            Args.ofString("bobnewpass")
        }));
        // 9. Bob views account
        requests.add(new Request(9, Constants.SERVICE_VIEW_ACCOUNT, new Args[]{
            Args.ofString("Bob"),
            Args.ofInt32(1),
            Args.ofString("bobnewpass")
        }));

        return requests;
    }
}

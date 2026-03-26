package client.testing;

import client.Constants;
import client.Marshalling;
import client.UDPClient;
import client.utils;
import java.net.SocketTimeoutException;
import java.nio.charset.StandardCharsets;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Queue;
import java.util.concurrent.TimeoutException;

public class SemanticInvocationLocalTest {
    private static final int OPEN_ACCOUNT_SERVICE_ID = 0;

    private static String toHex(byte[] data) {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < data.length; i++) {
            sb.append(String.format("%02X", data[i]));
            if (i < data.length - 1) {
                sb.append(' ');
            }
        }
        return sb.toString();
    }

    private static void printOpenAccountRequestBeforeMarshalling(
            int requestId,
            int serviceId,
            int statusCode,
            String userName,
            String password,
            String currency,
            long openingBalance) {
        System.out.println("[client] BEFORE marshalling request:");
        System.out.println("  requestId=" + requestId);
        System.out.println("  serviceId=" + serviceId);
        System.out.println("  statusCode=" + statusCode);
        System.out.println("  userName=" + userName);
        System.out.println("  password=" + password);
        System.out.println("  currency=" + currency);
        System.out.println("  openingBalance=" + openingBalance);
    }

    private static void printPacketAfterMarshalling(String prefix, byte[] packet) {
        System.out.println(prefix + " (len=" + packet.length + "):");
        System.out.println("  hex=" + toHex(packet));
    }

    private static final class FakeWire {
        private final Queue<byte[]> serverInbox = new ArrayDeque<>();
        private final Queue<byte[]> clientInbox = new ArrayDeque<>();
        private boolean droppedFirstClientRequest;
        private int ackCount;

        void sendClientRequest(byte[] payload, boolean allowDropFirstPacket) {
            if (allowDropFirstPacket && !this.droppedFirstClientRequest) {
                this.droppedFirstClientRequest = true;
                System.out.println("[wire] dropped first outbound request to simulate timeout");
                return;
            }
            this.serverInbox.offer(payload);
        }

        void sendClientAck(byte[] ackPayload) {
            this.ackCount += 1;
            this.serverInbox.offer(ackPayload);
        }

        byte[] readClientResponse() throws SocketTimeoutException {
            byte[] payload = this.clientInbox.poll();
            if (payload == null) {
                throw new SocketTimeoutException("simulated timeout waiting for server response");
            }
            return payload;
        }

        void sendServerResponse(byte[] payload) {
            this.clientInbox.offer(payload);
        }

        byte[] readServerInbound() {
            return this.serverInbox.poll();
        }

        boolean hasPendingClientResponse() {
            return !this.clientInbox.isEmpty();
        }

        int getAckCount() {
            return this.ackCount;
        }
    }

    private static final class FakeServer {
        private final Map<Integer, byte[]> responseCache = new HashMap<>();

        void processNextInbound(FakeWire wire) {
            byte[] payload = wire.readServerInbound();
            if (payload == null) {
                return;
            }

            if (isAck(payload)) {
                int ackResponseId = Marshalling.unmarshalInt32(payload, 0);
                System.out.println("[server] received ACK for responseId=" + ackResponseId);
                return;
            }

            utils.RequestHeader header = utils.parseRequestHeader(payload);
            int requestId = header.requestId;

            if (this.responseCache.containsKey(requestId)) {
                wire.sendServerResponse(this.responseCache.get(requestId));
                System.out.println("[server] duplicate request detected, re-sent cached response");
                return;
            }

            int[] counter = new int[] {Constants.INT_SIZE * 3};
            List<utils.Argument> requestArgs = utils.parseArgs(payload, counter, utils.schemaForService(OPEN_ACCOUNT_SERVICE_ID));

            String userName = requestArgs.get(0).stringValue;
            String password = requestArgs.get(1).stringValue;
            String currency = requestArgs.get(2).stringValue;
            long openingBalance = requestArgs.get(3).int64Value;

            System.out.println("[server] open-account request parsed: user=" + userName
                    + ", password=" + password + ", currency=" + currency
                    + ", openingBalance=" + openingBalance);

            byte[] responsePacket = buildOpenAccountResponse(requestId, 1001, userName, openingBalance);
            this.responseCache.put(requestId, responsePacket);

            wire.sendServerResponse(responsePacket);
            wire.sendServerResponse(responsePacket);
            System.out.println("[server] response marshalled and sent twice (to test duplicate handling)");
        }

        private boolean isAck(byte[] payload) {
            if (payload.length != Constants.INT_SIZE + Constants.ACK_CHAR.length()) {
                return false;
            }
            String ackByte = new String(payload, Constants.INT_SIZE, Constants.ACK_CHAR.length(), StandardCharsets.UTF_8);
            return Constants.ACK_CHAR.equals(ackByte);
        }

        private byte[] buildOpenAccountResponse(int responseId, int accountNumber, String userName, long openingBalance) {
            System.out.println("[server] BEFORE marshalling response:");
            System.out.println("  responseId=" + responseId);
            System.out.println("  serviceId=" + OPEN_ACCOUNT_SERVICE_ID);
            System.out.println("  statusCode=0");
            System.out.println("  accountNumber=" + accountNumber);
            System.out.println("  userName=" + userName);
            System.out.println("  openingBalance=" + openingBalance);

            List<byte[]> parts = new ArrayList<>();
            parts.add(Marshalling.marshal(responseId));
            parts.add(Marshalling.marshal(OPEN_ACCOUNT_SERVICE_ID));
            parts.add(Marshalling.marshal(0));
            parts.add(Marshalling.marshal(accountNumber));

            byte[] userBytes = userName.getBytes(StandardCharsets.UTF_8);
            parts.add(Marshalling.marshal(userBytes.length));
            parts.add(userBytes);

            parts.add(Marshalling.marshal(openingBalance));
            byte[] packet = utils.concat(parts);
            printPacketAfterMarshalling("[server] AFTER marshalling response packet", packet);
            return packet;
        }
    }

    private static byte[] sendRequestWithSemanticPolicy(
            UDPClient communication,
            FakeWire wire,
            FakeServer server,
            byte[] requestPayload,
            boolean monitorMode) throws TimeoutException, SocketTimeoutException {
        int timeoutCount = 0;

        while (true) {
            try {
                boolean allowDropFirstPacket = timeoutCount == 0;
                wire.sendClientRequest(requestPayload, allowDropFirstPacket);
                server.processNextInbound(wire);

                byte[] responsePacket = wire.readClientResponse();
                byte[] body = handleClientResponsePacket(communication, wire, responsePacket, monitorMode);
                if (body != null) {
                    return body;
                }
            } catch (SocketTimeoutException timeout) {
                timeoutCount += 1;
                communication.shouldRetryAfterTimeout(timeoutCount);
            }
        }
    }

    private static byte[] handleClientResponsePacket(
            UDPClient communication,
            FakeWire wire,
            byte[] payload,
            boolean monitorMode) {
        utils.RequestHeader header = utils.parseRequestHeader(payload);
        int responseId = header.requestId;
        int bodyOffset = Constants.INT_SIZE * 3;

        if (communication.isDebug()) {
            System.out.println("[client] received responseId=" + responseId);
        }

        UDPClient.ResponseDecision decision = communication.evaluateResponse(responseId, monitorMode);

        if (decision.shouldAck) {
            wire.sendClientAck(utils.buildAckPayload(responseId));
            if (communication.isDebug()) {
                System.out.println("[client] ACK sent for responseId=" + responseId);
            }
        }

        if (decision.duplicate) {
            if (communication.isDebug()) {
                System.out.println("[client] duplicate response ignored");
            }
            return null;
        }

        return java.util.Arrays.copyOfRange(payload, bodyOffset, payload.length);
    }

    private static void printOpenAccountResponse(byte[] responseBody) {
        int cursor = 0;

        int accountNumber = Marshalling.unmarshalInt32(responseBody, cursor);
        cursor += Constants.INT_SIZE;

        int userLen = Marshalling.unmarshalInt32(responseBody, cursor);
        cursor += Constants.INT_SIZE;
        String userName = new String(responseBody, cursor, userLen, StandardCharsets.UTF_8);
        cursor += userLen;

        long openingBalance = Marshalling.unmarshalInt64(responseBody, cursor);

        System.out.println("[client] unmarshalled response body:");
        System.out.println("  accountNumber=" + accountNumber);
        System.out.println("  userName=" + userName);
        System.out.println("  openingBalance=" + openingBalance);
    }

    private static void printOpenAccountRequestAfterUnmarshalling(byte[] requestPayload) {
        utils.RequestHeader header = utils.parseRequestHeader(requestPayload);
        int[] counter = new int[] {Constants.INT_SIZE * 3};
        List<utils.Argument> requestArgs = utils.parseArgs(requestPayload, counter, utils.schemaForService(OPEN_ACCOUNT_SERVICE_ID));

        System.out.println("[server] AFTER unmarshalling request payload:");
        System.out.println("  requestId=" + header.requestId);
        System.out.println("  serviceId=" + header.serviceId);
        System.out.println("  statusCode=" + header.statusCode);
        System.out.println("  userName=" + requestArgs.get(0).stringValue);
        System.out.println("  password=" + requestArgs.get(1).stringValue);
        System.out.println("  currency=" + requestArgs.get(2).stringValue);
        System.out.println("  openingBalance=" + requestArgs.get(3).int64Value);
    }

    public static void main(String[] args) throws Exception {
        UDPClient communication = new UDPClient();
        communication.setDebug(true);
        communication.setSemanticInvoc(Constants.AT_MOST_ONE_SEM_INVO);
        communication.setTimeoutMs(Constants.DEFAULT_TIMEOUT);
        communication.setMaxRetry(3);

        FakeWire wire = new FakeWire();
        FakeServer server = new FakeServer();

        int requestId = communication.getNextRequestId();
        int statusCode = 0;
        String userName = "alice";
        String password = "password123";
        String currency = "USD";
        long openingBalance = 1000L;

        printOpenAccountRequestBeforeMarshalling(
                requestId,
                OPEN_ACCOUNT_SERVICE_ID,
            statusCode,
            userName,
            password,
            currency,
            openingBalance);

        byte[] requestPayload = utils.buildRequest(
            requestId,
            OPEN_ACCOUNT_SERVICE_ID,
            statusCode,
            userName,
            password,
            currency,
            openingBalance);
        printPacketAfterMarshalling("[client] AFTER marshalling request packet", requestPayload);
        printOpenAccountRequestAfterUnmarshalling(requestPayload);

        byte[] responseBody = sendRequestWithSemanticPolicy(
                communication,
                wire,
                server,
                requestPayload,
                false);

        printOpenAccountResponse(responseBody);

        server.processNextInbound(wire);

        if (wire.hasPendingClientResponse()) {
            byte[] duplicateResponse = wire.readClientResponse();
            byte[] duplicateBody = handleClientResponsePacket(communication, wire, duplicateResponse, false);
            System.out.println("[client] duplicate payload body is null: " + (duplicateBody == null));
        }

        server.processNextInbound(wire);
        server.processNextInbound(wire);

        System.out.println("[summary] total ACK packets sent by client: " + wire.getAckCount());
    }
}

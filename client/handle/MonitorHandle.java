package client.handle;

import client.net.UDPClient;
import client.protocol.Request;
import client.protocol.Response;
import client.protocol.Args;
import client.Constants;

import java.io.IOException;
import java.util.concurrent.TimeoutException;

public class MonitorHandle {

    private static final int SERVICE_ID = Constants.SERVICE_MONITOR_UPDATE;

    private final UDPClient client;

    // Temporary state variables
    private Integer interval;
    
    private boolean monitoringActive;

    public MonitorHandle(UDPClient client) {
        this.client = client;
    }

    public void handle() {
        try {
            this.INPUT_STATE();

            if (this.CONFIRMATION_STATE() == 0) {
                return;
            }
            // [string, string, string] -> name, account#, passw
            Args[] payload = new Args[]{
                Args.ofInt32(this.interval)// Assuming account is an integer ID
            };
            Response response = this.client.sendRequest(SERVICE_ID, payload);
            this.REQUEST_RESULT_STATE(response);
            this.LIVE_MONITORING_STATE();
            this.MONITOR_END();

        } catch (IOException e) {
            UI.printError("[I/O] Error occurred: " + e.getMessage());
        } catch (TimeoutException e) {
            UI.printError("[TIMEOUT] Server did not respond.");
        } finally {
            this.CLEAN();
            UI.drawSeparator();
            UI.prompt("Press Enter to return to menu...");
        }
    }
    

    private void INPUT_STATE() {
        UI.clearScreen();
        UI.drawBox("MONITOR UPDATE");
        UI.drawSeparator();
        UI.display("Max Interval: 1800 seconds (30 minutes)");
        UI.drawSeparator();
        
        this.interval = Integer.parseInt(UI.prompt("Monitor Interval    :"));
        
    }

    private int CONFIRMATION_STATE() {
        while (true) {
            UI.clearScreen();
            UI.drawBox("CONFIRM MONITOR SERVER REQUEST");
            UI.display("Interval:    " + this.interval);
            UI.drawSeparator();
            String confirm = UI.prompt("Confirm Monitor? (y/n)");

            switch (confirm.toLowerCase()) {
                case "y":
                    return 1;
                case "n":
                    UI.clearScreen();
                    UI.printSuccess("Close account request cancelled.");
                    return 0;
                default:
                    UI.printError("Invalid input. Please enter 'y' or 'n'.");
            }
        }
    }

    private void REQUEST_RESULT_STATE(Response response) {
        UI.clearScreen();
        UI.drawBox("MONITOR REQUEST RESULT");
        if (response.isSuccess()) {
            UI.printSuccess("Monitor Request Successful! Waiting for updates...");
        } else {
            // Assuming payload[0] contains the error message from server
            String errorMsg = (String) response.payload[0].value;
            UI.printError("Monitor Request Failed: " + errorMsg);
        }
    }
    
    private void LIVE_MONITORING_STATE() throws IOException {
        UI.clearScreen();
        UI.drawBox("LIVE MONITORING");
        UI.display("Connected to server for live updates. \nPress Ctrl+C to exit.");
        UI.drawSeparator();

        // Keep the client running to receive updates until user interrupts
        try {
            this.monitoringActive = true;
            this.client.monitorServer(this::ON_UPDATE_CALLBACK);
            Thread.sleep(1000); 
        } catch (InterruptedException e) {
            UI.printWarning("Monitoring session ended.");
            Thread.currentThread().interrupt();
        } finally {
            this.monitoringActive = false;
        }
    }

    public void ON_UPDATE_CALLBACK(Request callbackRequest) {
        int serviceId = callbackRequest.serviceId;
        
        if (serviceId == Constants.SERVICE_MONITOR_LIVE) {
            String message = (String) callbackRequest.payload[0].value;
            UI.display(message);
        }
        else if (serviceId == Constants.SERVICE_MONITOR_END) {
            String message = (String) callbackRequest.payload[0].value;
            UI.printWarning(message);
            this.client.setMonitorMode(false); // toggle off monitor mode to end session
        }
        else{
            UI.printError("Unexpected request ID received. Request ID: " + serviceId);
        }
    }

    private void MONITOR_END(){
        UI.drawSeparator();
        UI.printWarning("Monitor session has ended!");
    }

    private void CLEAN() {
        this.interval = null;
        this.monitoringActive = false;
        this.client.setMonitorMode(false);
    }


}
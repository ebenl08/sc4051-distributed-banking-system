package client.protocol;



import client.Constants;

public class ServiceSchemaFactory {

    public static ServiceSchema getServiceSchema(int serviceId) {
        switch (serviceId) {
            case Constants.SERVICE_OPEN_ACCOUNT: 
                // [string, string, char, int64] -> [string]
                return new ServiceSchema(
                    new Args.Type[]{
                        Args.Type.STRING, // name
                        Args.Type.STRING, // passw
                        Args.Type.CHAR,   // currency
                        Args.Type.INT64   // initial balance
                    },
                    new Args.Type[]{
                        Args.Type.INT32,
                    } // account#
                );

            case Constants.SERVICE_CLOSE_ACCOUNT: 
                // [string, string, string] -> [string]
                return new ServiceSchema(
                    new Args.Type[]{
                        Args.Type.STRING, // name
                        Args.Type.INT32, // account#
                        Args.Type.STRING  // passw
                    },
                    new Args.Type[]{} // response
                );

            case Constants.SERVICE_DEPOSIT_WITHDRAW_MONEY: 
                // [string, string, string, char, int64] -> [string, int64]
                return new ServiceSchema(
                    new Args.Type[]{
                        Args.Type.STRING, // name
                        Args.Type.INT32, // account#
                        Args.Type.STRING, // passw
                        Args.Type.CHAR,   // currency
                        Args.Type.INT64   // amount
                    },
                    new Args.Type[]{
                        Args.Type.INT64   // new balance
                    }
                );

            case Constants.SERVICE_TRANSFER_MONEY: 
                // [string, string, string, string, char, char, string, int64] -> [string, int32, int64, string]
                return new ServiceSchema(
                    new Args.Type[]{
                        Args.Type.STRING, // name1
                        Args.Type.INT32, // account#1
                        Args.Type.STRING, // name2
                        Args.Type.INT32, // account#2
                        Args.Type.CHAR,   // currency1
                        Args.Type.CHAR,   // currency2
                        Args.Type.STRING, // passw1
                        Args.Type.INT64   // amount
                    },
                    new Args.Type[]{
                        Args.Type.STRING, // name2
                        Args.Type.INT32,  // account#2
                        Args.Type.CHAR,   // currency2
                        Args.Type.FLOAT,  // exchange rate
                        Args.Type.INT64   // recipient remaining balance
                    }
                );

            case Constants.SERVICE_CHANGE_PASSWORD: 
                // [string, int32, string, string, string] -> [string]
                // Added 5th param for "retype new password" per your instructions
                return new ServiceSchema(
                    new Args.Type[]{
                        Args.Type.STRING, // name
                        Args.Type.INT32, // account#
                        Args.Type.STRING, // passw
                        Args.Type.STRING, // newpassw
                    },
                    new Args.Type[]{} // response
                );

            case Constants.SERVICE_VIEW_ACCOUNT: 
                // Left as is per "others not in this document"
                return new ServiceSchema(
                    new Args.Type[]{
                        Args.Type.STRING, // name
                        Args.Type.INT32, // account#
                        Args.Type.STRING  // passw
                    }, 
                    new Args.Type[]{
                        Args.Type.INT32, // account#
                        Args.Type.STRING, // name
                        Args.Type.CHAR,   // currency
                        Args.Type.INT64   // balance
                    }
                );

            case Constants.SERVICE_MONITOR_UPDATE: 
                // Left as is per "others not in this document"
                return new ServiceSchema(
                    new Args.Type[]{Args.Type.INT32}, // duration in seconds
                    new Args.Type[]{} 
                );

            default:
                throw new IllegalArgumentException("Unknown service ID: " + serviceId);
        }
    }
}

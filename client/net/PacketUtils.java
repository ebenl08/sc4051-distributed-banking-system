package client.net;


import client.protocol.ServiceSchema;
import client.protocol.StatusCode;

import java.util.ArrayList;
import java.util.List;

import client.Constants;
import client.Utils;
import client.protocol.Args;
import client.protocol.Request;
import client.protocol.Response;
import java.nio.charset.StandardCharsets;



public class PacketUtils {

    static boolean debug = true;
    
    public static byte[] buildRequestMessage(Request request) {
        List<byte[]> message = new ArrayList<>();
        message.add(Marshalling.marshal(request.requestId));
        message.add(Marshalling.marshal(request.serviceId));
        message.addAll(buildPayload(request.payload));
        return concat(message);
        
    }
    
    public static byte[] buildResponseMessage(Response response) {
        List<byte[]> message = new ArrayList<>();
        message.add(Marshalling.marshal(response.responseId));
        message.add(Marshalling.marshal((char) response.statusCode));
        message.addAll(buildPayload(response.payload));
        return concat(message);
    }

    public static Request parseCallbackMessage(byte[] buffer) {
        int offset = 0;
        int requestId = Marshalling.unmarshalInt32(buffer, offset);
        offset += Constants.INT_SIZE;
        int serviceId = Marshalling.unmarshalInt32(buffer, offset);
        offset += Constants.INT_SIZE;
        
        Args.Type[] inputSchema = new Args.Type[]{Args.Type.STRING}; // For simplicity, we assume callback only has string argument
        Args[] args = parsePayload(buffer, offset, inputSchema);
        return new Request(requestId, serviceId, args);
    }

    public static  Response parseResponseMessage(byte[] buffer, ServiceSchema serviceSchema) {
        int offset = 0;
        int requestId = Marshalling.unmarshalInt32(buffer, offset); 
        
        offset += Constants.INT_SIZE;

        if(debug) {
            System.out.println("Raw bytes of string argument: " + Utils.bytesToHex(buffer));
            System.out.println("Parsed response ID: " + requestId);
            System.out.println("Offset after reading request ID: " + offset);
        }

        char statusCode = Marshalling.unmarshalChar(buffer, offset);
        System.out.println("Parsing char at offset " + offset + ": " + Utils.bytesToHex(new byte[]{buffer[offset]}));
        offset += Constants.CHAR_SIZE;
        
        if(debug) {
            System.out.println("Parsed status code: " + statusCode);
        }
        
        Args[] args;
        if (statusCode == StatusCode.SUCCESS) {
            args = parsePayload(buffer, offset, serviceSchema.outputSchema);
        } else {
            args = parsePayload(buffer, offset, serviceSchema.errorSchema);
        }

        if(debug) {
            System.out.println("Number of arguments parsed: " + args.length);
            System.out.println("Parsed arguments:");
            for (Args arg : args) {
                System.out.println("  " + arg);
            }
        }
        return new Response(requestId, statusCode, args);
    }


    private static Args[] parsePayload(byte[] buffer, int offset, Args.Type[] schema){ 
        
        List<Args> args = new ArrayList<>();
        
        for (Args.Type type : schema) {
            // For simplicity, we assume the first byte of each argument indicates its type
            if(offset >= buffer.length){
                break;
            }
            System.out.println("Parsing argument of type " + type + " at offset " + offset);

            switch (type) {
                case Args.Type.STRING:
                    int length = Marshalling.unmarshalInt32(buffer, offset);
                    offset += Constants.INT_SIZE; // Move past the length field
                    String strValue = new String(buffer, offset, length, StandardCharsets.UTF_8);
                    offset += length; // Move past the string data
                    args.add(Args.ofString(strValue));
                    break;
                case Args.Type.INT32:
                    int intValue = Marshalling.unmarshalInt32(buffer, offset);
                    offset += Constants.INT_SIZE; // Move past the int32 data
                    args.add(Args.ofInt32(intValue));
                    break;
                case Args.Type.INT64:
                    long longValue = Marshalling.unmarshalInt64(buffer, offset);
                    offset += Constants.LONG_SIZE; // Move past the int64 data
                    args.add(Args.ofInt64(longValue));
                    break;
                case Args.Type.FLOAT:
                    float floatValue = Marshalling.unmarshalFloat(buffer, offset);
                    offset += Constants.FLOAT_SIZE; // Move past the float data
                    args.add(Args.ofFloat(floatValue));
                    break;
                case Args.Type.CHAR:
                    char charValue = Marshalling.unmarshalChar(buffer, offset);
                    offset += Constants.CHAR_SIZE; // Move past the char data
                    args.add(Args.ofChar(charValue));
                    break;
                default:
                    throw new IllegalArgumentException("Unsupported argument type: " + type);
            }
        }
        return args.toArray(new Args[0]);
    }


    private static List<byte[]> buildPayload(Args[] args){
        
        List<byte[]> payload = new ArrayList<>();
        for(Args arg: args){
            switch(arg.type){
                case Args.Type.STRING:
                    payload.add(Marshalling.marshal((String) arg.value));
                    break;
                case Args.Type.CHAR:
                    payload.add(Marshalling.marshal((char) arg.value));
                    break;
                case Args.Type.INT32:
                    payload.add(Marshalling.marshal((int) arg.value));
                    break;
                case Args.Type.INT64:
                    payload.add(Marshalling.marshal((long) arg.value));
                    break;
                case Args.Type.FLOAT:
                    payload.add(Marshalling.marshal((float) arg.value));
                    break;
                case null:
                    throw new IllegalArgumentException("Argument type cannot be null");
                default:
                    throw new IllegalArgumentException("Unsupported argument type: " + arg.type);
            }
        }
        return payload;
    }

    private static byte[] concat(List<byte[]> parts){
        int total = 0;
		for(byte[] part : parts) total += part.length;

		byte[] out = new byte[total];
		int offset = 0;

		for(byte[] part : parts){
			System.arraycopy(part, 0, out, offset, part.length);
			offset += part.length;
		}
		return out;
    }
}

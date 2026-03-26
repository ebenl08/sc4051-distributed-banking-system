package client;

import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.List;

public final class utils {
	public enum DataType {
		TYPE_STRING,
		TYPE_INT32,
		TYPE_INT64,
		TYPE_FLOAT
	}

	public static final class Argument {
		public final DataType type;
		public final String stringValue;
		public final Integer int32Value;
		public final Long int64Value;
		public final Float floatValue;

		private Argument(DataType type, String stringValue, Integer int32Value, Long int64Value, Float floatValue) {
			this.type = type;
			this.stringValue = stringValue;
			this.int32Value = int32Value;
			this.int64Value = int64Value;
			this.floatValue = floatValue;
		}

		public static Argument ofString(String value) {
			return new Argument(DataType.TYPE_STRING, value, null, null, null);
		}

		public static Argument ofInt32(int value) {
			return new Argument(DataType.TYPE_INT32, null, value, null, null);
		}

		public static Argument ofInt64(long value) {
			return new Argument(DataType.TYPE_INT64, null, null, value, null);
		}

		public static Argument ofFloat(float value) {
			return new Argument(DataType.TYPE_FLOAT, null, null, null, value);
		}
	}

	public static final class RequestHeader {
		public final int requestId;
		public final int serviceId;
		public final int statusCode;

		public RequestHeader(int requestId, int serviceId, int statusCode) {
			this.requestId = requestId;
			this.serviceId = serviceId;
			this.statusCode = statusCode;
		}
	}

	private utils() {}

	public static byte[] concat(List<byte[]> parts){
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

	public static byte[] buildRequest(int requestId, int serviceId, int statusCode, Object... fields){
		List<byte[]> parts = new ArrayList<>();

		parts.add(Marshalling.marshal(requestId));
		parts.add(Marshalling.marshal(serviceId));
		parts.add(Marshalling.marshal(statusCode));

		for(Object field : fields){
			if(field instanceof Integer integer){
				parts.add(Marshalling.marshal(Constants.INT_SIZE));
				parts.add(Marshalling.marshal(integer));
			} else if(field instanceof Long longValue){
				parts.add(Marshalling.marshal(Constants.LONG_SIZE));
				parts.add(Marshalling.marshal(longValue));
			} else if(field instanceof Float floatValue){
				parts.add(Marshalling.marshal(Constants.FLOAT_SIZE));
				parts.add(Marshalling.marshal(floatValue));
			} else if(field instanceof String string){
				byte[] strBytes = string.getBytes(StandardCharsets.UTF_8);
				parts.add(Marshalling.marshal(strBytes.length));
				parts.add(strBytes);
			} else {
				throw new IllegalArgumentException("Unsupported field type: " + field.getClass().getName());
			}
		}

		return concat(parts);
	}
	public static byte[] buildAckPayload(int responseId){
		List<byte[]> parts = new ArrayList<>();
		parts.add(Marshalling.marshal(responseId));
		parts.add(Constants.ACK_CHAR.getBytes(StandardCharsets.UTF_8));
		return concat(parts);
	}

	public static int readResponseId(byte[] responsePacket) {
		return Marshalling.unmarshalInt32(responsePacket, 0);
	}

	public static byte[] readResponseBody(byte[] responsePacket) {
		byte[] body = new byte[responsePacket.length - Constants.INT_SIZE];
		System.arraycopy(responsePacket, Constants.INT_SIZE, body, 0, body.length);
		return body;
	}


	public static RequestHeader parseRequestHeader(byte[] buffer) {
		int reqId = Marshalling.unmarshalInt32(buffer, 0);
		int serviceId = Marshalling.unmarshalInt32(buffer, Constants.INT_SIZE);
		int statusCode = Marshalling.unmarshalInt32(buffer, Constants.INT_SIZE * 2);
		return new RequestHeader(reqId, serviceId, statusCode);
	}

	public static List<Argument> parseArgs(byte[] buffer, int[] counterRef, List<DataType> schema) {
		if (counterRef == null || counterRef.length == 0) {
			throw new IllegalArgumentException("counterRef must contain one mutable counter slot");
		}

		int counter = counterRef[0];
		List<Argument> args = new ArrayList<>();

		for (DataType type : schema) {
			switch (type) {
				case TYPE_STRING:
					int strLength = Marshalling.unmarshalInt32(buffer, counter);
					counter += Constants.INT_SIZE;
					String stringValue = new String(buffer, counter, strLength, StandardCharsets.UTF_8);
					counter += strLength;
					args.add(Argument.ofString(stringValue));
					break;
				case TYPE_INT32:
					int int32Size = Marshalling.unmarshalInt32(buffer, counter);
					counter += Constants.INT_SIZE;
					if (int32Size != Constants.INT_SIZE) {
						throw new IllegalArgumentException("Invalid int32 size prefix: " + int32Size);
					}
					int intValue = Marshalling.unmarshalInt32(buffer, counter);
					counter += Constants.INT_SIZE;
					args.add(Argument.ofInt32(intValue));
					break;
				case TYPE_INT64:
					int int64Size = Marshalling.unmarshalInt32(buffer, counter);
					counter += Constants.INT_SIZE;
					if (int64Size != Constants.LONG_SIZE) {
						throw new IllegalArgumentException("Invalid int64 size prefix: " + int64Size);
					}
					long longValue = Marshalling.unmarshalInt64(buffer, counter);
					counter += Constants.LONG_SIZE;
					args.add(Argument.ofInt64(longValue));
					break;
				case TYPE_FLOAT:
					int floatSize = Marshalling.unmarshalInt32(buffer, counter);
					counter += Constants.INT_SIZE;
					if (floatSize != Constants.FLOAT_SIZE) {
						throw new IllegalArgumentException("Invalid float size prefix: " + floatSize);
					}
					float floatValue = Marshalling.unmarshalFloat(buffer, counter);
					counter += Constants.FLOAT_SIZE;
					args.add(Argument.ofFloat(floatValue));
					break;
				default:
					throw new IllegalArgumentException("Unsupported schema type: " + type);
			}
		}

		counterRef[0] = counter;
		return args;
	}

	public static List<DataType> schemaForService(int serviceId) {
		switch (serviceId) {
			//Open New Account
			case 0:
				return List.of(DataType.TYPE_STRING, DataType.TYPE_STRING, DataType.TYPE_STRING, DataType.TYPE_INT64);
			//Close Account
			case 1:
				return List.of(DataType.TYPE_STRING, DataType.TYPE_INT32, DataType.TYPE_STRING);
			//Deposit/Withdraw 
			//(need to change the currency to char/uint_8 later)
			case 2:
				return List.of(DataType.TYPE_STRING, DataType.TYPE_INT32, DataType.TYPE_STRING, DataType.TYPE_STRING, DataType.TYPE_INT64);
			//Transfer Money
			//(need to change the currency to char/uint_8 later)
			case 3:
				return List.of(DataType.TYPE_STRING, DataType.TYPE_INT32, DataType.TYPE_STRING, DataType.TYPE_INT32, DataType.TYPE_STRING, DataType.TYPE_STRING, DataType.TYPE_STRING, DataType.TYPE_INT64);
			//change password 
			case 4:
				return List.of(DataType.TYPE_STRING, DataType.TYPE_INT32, DataType.TYPE_STRING, DataType.TYPE_STRING);
			case 5:
				return List.of();
			default:
				throw new IllegalArgumentException("Unknown serviceId: " + serviceId);
		}
	}

	public static List<Argument> decodeByService(byte[] buffer, int[] counterRef, int serviceId) {
		return parseArgs(buffer, counterRef, schemaForService(serviceId));
	}

	public static List<Argument> decodeResponseBodyByService(byte[] responseBody, int serviceId) {
		int[] counterRef = new int[] {0};
		return decodeByService(responseBody, counterRef, serviceId);
	}
	
}

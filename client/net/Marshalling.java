package client.net;

import java.nio.*;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;

public class Marshalling {
  public static byte[] marshal(int value){
    ByteBuffer buffer = ByteBuffer.wrap(new byte[]{
      (byte) (value >> 24), 
      (byte) (value >> 16),
      (byte) (value >> 8),
      (byte) (value),
    }).order(ByteOrder.BIG_ENDIAN);
    return buffer.array();
  }

  public static byte[] marshal(long value){
    ByteBuffer buffer = ByteBuffer.wrap(new byte[]{
      (byte) (value >> 56), 
      (byte) (value >> 48),
      (byte) (value >> 40),
      (byte) (value >> 32),
      (byte) (value >> 24), 
      (byte) (value >> 16),
      (byte) (value >> 8),
      (byte) (value),
    }).order(ByteOrder.BIG_ENDIAN);

    return buffer.array();
  }

  public static byte[] marshal(float value){
    int bits = Float.floatToIntBits(value);
    return marshal(bits);
  }

  public static byte[] marshal(String value){
    byte[] buffer = new byte[4 + value.length()];
    
    for(int i = 0; i < value.length(); i++){
      buffer[i + 4] = (byte) value.charAt(i);
    }
    return ByteBuffer.wrap(buffer)
        .order(ByteOrder.BIG_ENDIAN)
        .array();
  }
  
  // Java byte is signed so
  public static int unmarshalInt32(byte[] buffer, int offset){
    buffer = ByteBuffer.wrap(buffer).order(ByteOrder.LITTLE_ENDIAN).array();
    return (int) (buffer[offset] & 0xFF) << 24
         | (buffer[offset + 1] & 0xFF) << 16 
         | (buffer[offset + 2] & 0xFF) << 8 
         | (buffer[offset + 3] & 0xFF);
  }

  public static long unmarshalInt64(byte[] buffer, int offset){
    buffer = ByteBuffer.wrap(buffer).order(ByteOrder.LITTLE_ENDIAN).array();
    return ((long) buffer[offset] & 0xFF) << 56
         | ((long) buffer[offset + 1] & 0xFF) << 48
         | ((long) buffer[offset + 2] & 0xFF) << 40 
         | ((long) buffer[offset + 3] & 0xFF) << 32
         | ((long) buffer[offset + 4] & 0xFF) << 24 
         | ((long) buffer[offset + 5] & 0xFF) << 16
         | ((long) buffer[offset + 6] & 0xFF) << 8 
         | ((long) buffer[offset + 7] & 0xFF);
  }

  public static float unmarshalFloat(byte[] buffer, int offset){
    int temp = unmarshalInt32(buffer, offset);
    return Float.intBitsToFloat(temp);
  }

  public static String unmarshalString(byte[] buffer, int offset){
    buffer = ByteBuffer.wrap(buffer).order(ByteOrder.LITTLE_ENDIAN).array();
    int length = unmarshalInt32(buffer, offset);
    offset += 4;

    String value = new String(buffer, StandardCharsets.UTF_8);
    for(int i = 0; i < length; i++){
      value += buffer[offset + i];
    }
    return value;
  }

  public static void main(String[] args) {
    int val32 = 0x180095AB;
    long val64 = 0x180095AB180095ABL;
    float valf = 3.14f;
    String valstr = "Hello I am a client";
    
    byte[] buffer;

    System.out.printf("0x%08x%n", val32);
    buffer = Marshalling.marshal(val32);
    val32 = Marshalling.unmarshalInt32(buffer, 0);
    Arrays.fill(buffer, (byte) 0);
    System.out.printf("0x%08x%n", val32);
    
    
    System.out.printf("0x%016x%n", val64);
    buffer = Marshalling.marshal(val64);
    val64 = Marshalling.unmarshalInt64(buffer, 0);
    Arrays.fill(buffer, (byte) 0);
    System.out.printf("0x%016x%n", val64);
    
    
    System.out.printf("%a%n", valf);
    buffer = Marshalling.marshal(valf);
    valf = Marshalling.unmarshalFloat(buffer, 0);
    Arrays.fill(buffer, (byte) 0);
    System.out.printf("%a%n", valf);


    System.out.printf("%s%n", valstr);
    buffer = Marshalling.marshal(valstr);
    valstr = Marshalling.unmarshalString(buffer, 0);
    Arrays.fill(buffer, (byte) 0);
    System.out.printf("%s%n", valstr);

  }
}

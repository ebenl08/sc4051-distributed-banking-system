package client.protocol;



public class ServiceSchema {

    // Service Schema only defines the input and output types of a service, it does not contain the actual values
    // This does not include error type as it is considered trivial (it is the same across services)
    
    public final Args.Type[] inputSchema;
    public final Args.Type[] outputSchema;
    public final Args.Type[] errorSchema;

    public ServiceSchema(Args.Type[] inputSchema, Args.Type[] outputSchema) {
        this.inputSchema = inputSchema;
        this.outputSchema = outputSchema;
        this.errorSchema = new Args.Type[]{Args.Type.STRING}; // error message
    }

}

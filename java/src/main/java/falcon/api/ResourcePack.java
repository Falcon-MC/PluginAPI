package falcon.api;

/** A Bedrock resource pack archive, with an optional encryption key. */
public record ResourcePack(byte[] data, String contentKey) {
    public ResourcePack(byte[] data) {
        this(data, null);
    }
}

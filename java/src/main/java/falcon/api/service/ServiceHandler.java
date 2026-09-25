package falcon.api.service;

@FunctionalInterface
public interface ServiceHandler {
    String handle(String request);
}

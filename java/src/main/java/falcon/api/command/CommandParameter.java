package falcon.api.command;

import java.util.List;

/**
 * One parameter of a command overload. The values of an enum parameter are the choices the client completes.
 */
public record CommandParameter(String name, CommandParamType type, boolean optional, List<String> values) {
    public static CommandParameter of(String name, CommandParamType type) {
        return new CommandParameter(name, type, false, List.of());
    }

    public static CommandParameter optional(String name, CommandParamType type) {
        return new CommandParameter(name, type, true, List.of());
    }

    public static CommandParameter choice(String name, List<String> values) {
        return new CommandParameter(name, CommandParamType.ENUM, false, List.copyOf(values));
    }
}

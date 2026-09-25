package falcon.api.command;

@FunctionalInterface
public interface CommandHandler {
    boolean handle(CommandContext context);
}

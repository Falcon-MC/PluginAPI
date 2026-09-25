package falcon.api.command;

import falcon.api.Player;
import falcon.api.internal.Interop;
import falcon.api.internal.PluginContext;

import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.util.List;
import java.util.Optional;

public final class CommandContext {
    private final PluginContext mContext;
    private final MemorySegment mSender;
    private final List<String> mArguments;

    CommandContext(PluginContext context, MemorySegment sender, List<String> arguments) {
        mContext = context;
        mSender = sender;
        mArguments = List.copyOf(arguments);
    }

    public String senderName() {
        return Interop.string(Interop.api().senderName(mSender));
    }

    public Optional<Player> player() {
        MemorySegment player = Interop.api().senderPlayer(mSender);
        if (Interop.isNull(player)) {
            return Optional.empty();
        }
        return Optional.of(new Player(mContext, player));
    }

    public List<String> arguments() {
        return mArguments;
    }

    public boolean hasPermission(String node) {
        try (Arena arena = Arena.ofConfined()) {
            return Interop.api().senderHasPermission(mSender, Interop.text(arena, node)) != 0;
        }
    }

    public void reply(String message) {
        try (Arena arena = Arena.ofConfined()) {
            Interop.api().senderSendMessage(mSender, Interop.text(arena, message));
        }
    }
}

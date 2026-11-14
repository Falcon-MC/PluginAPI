package falcon.api.command;

import falcon.api.internal.FalconAbi;
import falcon.api.internal.Interop;
import falcon.api.internal.PluginContext;
import falcon.api.internal.Upcalls;

import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.lang.foreign.ValueLayout;
import java.lang.invoke.MethodHandles;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

public final class Commands {
    private static final MemorySegment DISPATCH =
            Upcalls.stub(MethodHandles.lookup(), "dispatch", FalconAbi.COMMAND_HANDLER);

    private final PluginContext mContext;

    public Commands(PluginContext context) {
        mContext = context;
    }

    public boolean add(String name, String description, String usage, CommandHandler handler) {
        return add(name, description, usage, handler, CommandPermission.OPERATOR);
    }

    public boolean add(String name, String description, String usage, CommandHandler handler,
                       CommandPermission permission) {
        Objects.requireNonNull(handler, "handler");
        MemorySegment userData = Upcalls.register(new Registration(mContext, handler));
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment descriptor = arena.allocate(FalconAbi.CommandDescriptor.LAYOUT);
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.CommandDescriptor.NAME, Interop.text(arena, name));
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.CommandDescriptor.DESCRIPTION,
                    Interop.text(arena, description));
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.CommandDescriptor.USAGE, Interop.text(arena, usage));
            descriptor.set(ValueLayout.JAVA_INT, FalconAbi.CommandDescriptor.PERMISSION, permission.value());
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.CommandDescriptor.HANDLER, DISPATCH);
            descriptor.set(ValueLayout.ADDRESS, FalconAbi.CommandDescriptor.USER_DATA, userData);

            boolean added = Interop.api().registerCommand(mContext.handle(), descriptor) != 0;
            if (!added) {
                Upcalls.release(userData, Registration.class);
            }
            return added;
        }
    }

    /**
     * Adds a typed signature to a command of this plugin; the client shows it and completes its arguments. The handler
     * still receives the arguments as text, one per word. The first overload replaces the default one.
     */
    public boolean addOverload(String command, List<CommandParameter> parameters) {
        try (Arena arena = Arena.ofConfined()) {
            long size = FalconAbi.CommandParameter.LAYOUT.byteSize();
            MemorySegment array = arena.allocate(FalconAbi.CommandParameter.LAYOUT, Math.max(parameters.size(), 1));
            for (int index = 0; index < parameters.size(); index++) {
                CommandParameter parameter = parameters.get(index);
                MemorySegment entry = array.asSlice(index * size, size);
                MemorySegment values = arena.allocate(ValueLayout.ADDRESS, Math.max(parameter.values().size(), 1));
                for (int value = 0; value < parameter.values().size(); value++) {
                    values.setAtIndex(ValueLayout.ADDRESS, value, Interop.text(arena, parameter.values().get(value)));
                }
                entry.set(ValueLayout.ADDRESS, FalconAbi.CommandParameter.NAME, Interop.text(arena, parameter.name()));
                entry.set(ValueLayout.JAVA_INT, FalconAbi.CommandParameter.TYPE, parameter.type().value());
                entry.set(ValueLayout.JAVA_INT, FalconAbi.CommandParameter.OPTIONAL, parameter.optional() ? 1 : 0);
                entry.set(ValueLayout.ADDRESS, FalconAbi.CommandParameter.VALUES, values);
                entry.set(ValueLayout.JAVA_INT, FalconAbi.CommandParameter.VALUE_COUNT, parameter.values().size());
            }
            return Interop.api().commandAddOverload(mContext.handle(), Interop.text(arena, command), array,
                    parameters.size()) != 0;
        }
    }

    /**
     * Runs a command line, without the slash, as the server console.
     */
    public boolean execute(String commandLine) {
        try (Arena arena = Arena.ofConfined()) {
            return Interop.api().serverExecuteCommand(Interop.text(arena, commandLine)) != 0;
        }
    }

    private static int dispatch(MemorySegment sender, MemorySegment arguments, int argumentCount,
                                MemorySegment userData) {
        try {
            Registration registration = Upcalls.target(userData, Registration.class);
            if (registration == null) {
                return 0;
            }

            int count = Math.max(argumentCount, 0);
            MemorySegment array = arguments.reinterpret(ValueLayout.ADDRESS.byteSize() * count);
            List<String> values = new ArrayList<>(count);
            for (int index = 0; index < count; index++) {
                values.add(Interop.string(array.getAtIndex(ValueLayout.ADDRESS, index)));
            }

            CommandContext context = new CommandContext(registration.context(), sender, values);
            String where = "A command handler threw an exception";
            return registration.context().test(where, () -> registration.handler().handle(context)) ? 1 : 0;
        } catch (Throwable ignored) {
            return 0;
        }
    }

    private record Registration(PluginContext context, CommandHandler handler) {
    }
}

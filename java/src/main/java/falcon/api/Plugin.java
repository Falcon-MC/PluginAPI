package falcon.api;

import falcon.api.command.Commands;
import falcon.api.content.Content;
import falcon.api.event.Events;
import falcon.api.internal.FalconAbi;
import falcon.api.internal.Interop;
import falcon.api.internal.PluginContext;
import falcon.api.service.Services;

import java.lang.foreign.Arena;
import java.lang.foreign.MemorySegment;
import java.lang.foreign.ValueLayout;
import java.nio.file.Path;
import java.util.List;

public abstract class Plugin {
    private final PluginContext mContext;
    private final Logger mLogger;
    private final Server mServer;
    private final Events mEvents;
    private final Commands mCommands;
    private final Scheduler mScheduler;
    private final Permissions mPermissions;
    private final Content mContent;
    private final Services mServices;
    private final PersistentData mData;
    private Config mConfig;

    protected Plugin() {
        mContext = PluginContext.claim();
        mLogger = new Logger(mContext);
        mServer = new Server(mContext);
        mEvents = new Events(mContext);
        mCommands = new Commands(mContext);
        mScheduler = new Scheduler(mContext);
        mPermissions = new Permissions(mContext);
        mContent = new Content(mContext);
        mServices = new Services(mContext);
        mData = new PersistentData(mContext);
    }

    public void onLoad() {
    }

    public boolean onEnable() {
        return true;
    }

    public void onDisable() {
    }

    public final String name() {
        return Interop.string(Interop.api().pluginName(mContext.handle()));
    }

    public final Path dataFolder() {
        return Path.of(Interop.string(Interop.api().pluginDataFolder(mContext.handle())));
    }

    public final Logger logger() {
        return mLogger;
    }

    public final Server server() {
        return mServer;
    }

    public final Events events() {
        return mEvents;
    }

    public final Commands commands() {
        return mCommands;
    }

    public final Scheduler scheduler() {
        return mScheduler;
    }

    public final Permissions permissions() {
        return mPermissions;
    }

    public final Content content() {
        return mContent;
    }

    public final Services services() {
        return mServices;
    }

    public final PersistentData data() {
        return mData;
    }

    public final Config config() {
        if (mConfig == null) {
            mConfig = new Config(dataFolder().resolve("config.yml"));
            mConfig.load();
        }
        return mConfig;
    }

    public final void saveConfig() {
        config().save();
    }

    /** Replaces this plugin's resource packs for future logins. Call from the server thread. */
    public final boolean replaceResourcePacks(List<ResourcePack> packs, boolean required) {
        List<ResourcePack> stack = List.copyOf(packs);
        long total = 0;
        if (stack.size() > 128) {
            return false;
        }
        for (ResourcePack pack : stack) {
            total += pack.data().length;
            if (pack.data().length == 0 || total > 128L * 1024 * 1024) {
                return false;
            }
        }
        try (Arena arena = Arena.ofConfined()) {
            MemorySegment descriptors = stack.isEmpty() ? MemorySegment.NULL
                    : arena.allocate(FalconAbi.ResourcePackDescriptor.LAYOUT, stack.size());
            for (int index = 0; index < stack.size(); index++) {
                ResourcePack pack = stack.get(index);
                MemorySegment descriptor = descriptors.asSlice(index * FalconAbi.ResourcePackDescriptor.LAYOUT.byteSize(),
                        FalconAbi.ResourcePackDescriptor.LAYOUT);
                descriptor.set(ValueLayout.ADDRESS, FalconAbi.ResourcePackDescriptor.DATA,
                        arena.allocateFrom(ValueLayout.JAVA_BYTE, pack.data()));
                descriptor.set(ValueLayout.JAVA_INT, FalconAbi.ResourcePackDescriptor.SIZE, pack.data().length);
                descriptor.set(ValueLayout.ADDRESS, FalconAbi.ResourcePackDescriptor.CONTENT_KEY,
                        Interop.nullableText(arena, pack.contentKey()));
            }
            return Interop.api().resourcePacksReplace(mContext.handle(), descriptors, stack.size(), required ? 1 : 0) != 0;
        }
    }

    public final MemorySegment handle() {
        return mContext.handle();
    }
}

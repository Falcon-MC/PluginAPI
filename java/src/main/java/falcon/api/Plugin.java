package falcon.api;

import falcon.api.command.Commands;
import falcon.api.content.Content;
import falcon.api.event.Events;
import falcon.api.internal.Interop;
import falcon.api.internal.PluginContext;
import falcon.api.service.Services;

import java.lang.foreign.MemorySegment;
import java.nio.file.Path;

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

    public final MemorySegment handle() {
        return mContext.handle();
    }
}
